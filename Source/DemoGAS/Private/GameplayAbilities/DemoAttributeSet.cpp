// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/DemoAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameplayTagsManager.h"
#include "GameFramework/Character.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "GameplayAbilities/Library/DemoSystemLibrary.h"
#include "Interface/CombatInterface.h"
#include "Interface/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/DemoPlayerController.h"
#include "Player/DemoPlayerState.h"


UDemoAttributeSet::UDemoAttributeSet()
{
}

void UDemoAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	//TODO 暂时留着吧，不知道有什么用
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UDemoAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//获取每次GE执行的各种信息
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	/*
	 *为什么不能在PreAttributeChange中Clamp数值？
	 * PreAttributeChange只会修改Modifier的返回的结果，并不会永久修改。
	 * 当其他的使用到Modifier时，仍然会得到Clamp之前的值
	 */
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	//meta attributes 自定义获得XP的逻辑
	if (Data.EvaluatedData.Attribute == GetInComingXPAttribute())
	{
		if (GetInComingXP() == 0.f)
		{
			return;
		}
		const int32 XP = GetInComingXP();
		SetInComingXP(0.f);
		if(Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>()) 
		{
			//添加XP之前
			int32 CurrentLevel = Cast<ICombatInterface>(Props.SourceCharacter)->GetPlayerLevel();
			int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
			int32 NewLevel = IPlayerInterface::Execute_FindLevelFromXP(Props.SourceCharacter,CurrentXP + XP);
			int32 NumOfLevelToUp = NewLevel - CurrentLevel;
			if (NumOfLevelToUp > 0)
			{
				IPlayerInterface::Execute_AddPlayerLevel(Props.SourceCharacter,NumOfLevelToUp);
				IPlayerInterface::Execute_AddLevelPoints(Props.SourceCharacter,NumOfLevelToUp);
			}
			//添加XP
			IPlayerInterface::Execute_AddXP(Props.SourceCharacter, XP);
		}
	}

	//meta attributes 自定义受到伤害的计算逻辑
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		if (GetInComingDamage() == 0.f)
		{
			return;
		}
		//缓存数值并重置
		const float LocalInComingDamage = GetInComingDamage();
		SetInComingDamage(0.f);

		//计算新血量
		const float NewHealth = GetHealth() - LocalInComingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		//通知Client显示伤害数字
		if (Props.SourceCharacter != Props.TargetCharacter)
		{
			//拿到造成伤害的远程控制器 (目前OnServer)
			if (Props.SourceController)
			{
				ADemoPlayerController* DemoPC = Cast<ADemoPlayerController>(Props.SourceController);
				DemoPC->ShowDamageNumber(LocalInComingDamage, UDemoSystemLibrary::IsCriticalHit(Props.GEContentHandle),
				                         Props.TargetCharacter);
			}
		}


		//死亡逻辑
		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvaterActor);
			if (CombatInterface)
			{
				CombatInterface->Die(Cast<UDemoAbilitySystemComponent>(Props.SourceASC));
				SendXPEvent(Props);
			}
		}
	}
}


void UDemoAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProps)
{
	//FGameplayEffectContentHandle
	OutProps.GEContentHandle = Data.EffectSpec.GetContext();
	//ASC
	OutProps.SourceASC = OutProps.GEContentHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(OutProps.SourceASC))
	{
		//AvatarActor
		OutProps.SourceAvaterActor = OutProps.SourceASC->GetAvatarActor();
		//先尝试使用ASC获取Controller，获取失败就用AvatarActor转换为Pawn来获取
		OutProps.SourceController = OutProps.SourceASC->AbilityActorInfo->PlayerController.Get(); //PC经常是null
		if (OutProps.SourceController == nullptr && OutProps.SourceAvaterActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(OutProps.SourceAvaterActor))
			{
				OutProps.SourceController = Pawn->GetController();
			}
		}
		//Character
		if (OutProps.SourceController)
		{
			OutProps.SourceCharacter = OutProps.SourceController->GetCharacter();
		}

		//Target
		OutProps.TargetAvaterActor = Data.Target.GetAvatarActor();
		OutProps.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		OutProps.TargetCharacter = Cast<ACharacter>(OutProps.TargetAvaterActor);
		OutProps.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OutProps.TargetAvaterActor);
	}
}

void UDemoAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (ICombatInterface* TargetCombatI = Cast<ICombatInterface>(Props.TargetCharacter))
	{
		int32 TargetLevel = TargetCombatI->GetPlayerLevel();
		//TODO：这里可以使用Curve或者别的来配置不同等级获得的经验值，我这里先直接处理了
		FGameplayEventData Payload;
		//TODO:这里的EventTag和SetByCaller中的Tag一样没问题吗?
		FGameplayTag EventTag = UGameplayTagsManager::Get().RequestGameplayTag("Attributes.Meta.InComingXP");
		//payload中填入的EventTag只是被传递后用做SetByCaller使用
		Payload.EventTag = EventTag;
		Payload.EventMagnitude = TargetLevel * 200.f;
		//这里的EventTag就是表示Event用的EventTag了
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			Props.SourceCharacter,
			EventTag,
			Payload);
	}
}

void UDemoAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, CriticalBoncePercent, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, AttackDistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet, SpellArmor, COND_None, REPNOTIFY_Always);
}

void UDemoAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	//向ASC通知属性完成了复制，TODO(用于预测回滚等?)
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, Health, OldValue)
}

void UDemoAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, MaxHealth, OldValue);
}

void UDemoAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, Mana, OldValue);
}

void UDemoAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, MaxMana, OldValue);
}

void UDemoAttributeSet::OnRep_AttackDistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, AttackDistance, OldValue);
}

void UDemoAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, Strength, OldValue);
}

void UDemoAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, Intelligence, OldValue);
}

void UDemoAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, Resilience, OldValue);
}

void UDemoAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, Armor, OldValue);
}

void UDemoAttributeSet::OnRep_SpellArmor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, SpellArmor, OldValue);
}

void UDemoAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, CriticalChance, OldValue);
}

void UDemoAttributeSet::OnRep_CriticalBoncePercent(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, CriticalBoncePercent, OldValue);
}

void UDemoAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, HealthRegeneration, OldValue);
}

void UDemoAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet, ManaRegeneration, OldValue);
}
