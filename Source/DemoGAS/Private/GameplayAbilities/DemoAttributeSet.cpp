// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/DemoAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


UDemoAttributeSet::UDemoAttributeSet()
{
	//初始化Attribute
	InitHealth(10);
	InitMaxHealth(100);
	InitMana(10);
	InitMaxMana(100);
}

void UDemoAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
}

void UDemoAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//获取每次GE执行的各种信息
	FEffectProperties Props;
	SetEffectProperties(Data,Props);

	/*为什么不能在PreAttributeChange中Clamp数值？
	 * PreAttributeChange只会修改Modifier的返回的结果，并不会永久修改。
	 * 当其他的使用到Modifier时，仍然会得到Clamp之前的值
	 */
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
	}
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
}


void UDemoAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& OutProps)
{
	//FGameplayEffectContentHandle
	OutProps.GEContentHandle = Data.EffectSpec.GetContext();
	//ASC
	OutProps.SourceASC = OutProps.GEContentHandle.GetOriginalInstigatorAbilitySystemComponent();

	if(IsValid(OutProps.SourceASC))
	{
		//AvatarActor
		OutProps.SourceAvaterActor = OutProps.SourceASC->GetAvatarActor();
		//先尝试使用ASC获取Controller，获取失败就用AvatarActor转换为Pawn来获取
		OutProps.SourceController = OutProps.SourceASC->AbilityActorInfo->PlayerController.Get();//PC经常是null
		if(OutProps.SourceController == nullptr && OutProps.SourceAvaterActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(OutProps.SourceAvaterActor))
			{
				OutProps.SourceController = Pawn->GetController();
			}
		}
		//Character
		if(OutProps.SourceController)
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

void UDemoAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,Health,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
}

void UDemoAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	//向ASC通知属性完成了复制，TODO(用于预测回滚等?)
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet,Health,OldValue)
}

void UDemoAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet,MaxHealth,OldValue); 
}

void UDemoAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet,Mana,OldValue);
}

void UDemoAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet,MaxMana,OldValue);
}

void UDemoAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet,Strength,OldValue);
}

void UDemoAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet,Intelligence,OldValue);
}

void UDemoAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet,Resilience,OldValue);
}

void UDemoAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDemoAttributeSet,Vigor,OldValue);
}
