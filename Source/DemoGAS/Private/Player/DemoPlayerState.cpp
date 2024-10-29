// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DemoPlayerState.h"

#include "Character/DemoPlayerCharacter.h"
#include "Game/DemoGameModeBase.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "GameplayAbilities/DemoAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ADemoPlayerState::ADemoPlayerState()
{
	//同步到Client的频率，PlayerState默认频率很低。
	//因为要在PlayerState中包含AbilitySystemComponent和AttributesSet，需要拉高
	NetUpdateFrequency = 100.f;

	//构造ASC和AttributeSet
	AbilitySystemComponent = CreateDefaultSubobject<UDemoAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true); //TODO(为什么只给ASC设置复制?)
	//Full会把GE复制给所有人   Mixed只会把GE复制给拥有它的Client（可以更新HUD等）  Minimal不会复制GE
	//但是GameplayCue和GameplayTags一直都会被复制
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	//在ASC的OwnerActor的构造函数中创建AttributeSet时，会自动注册AttributeSet到ASC组件中
	AttributeSet = CreateDefaultSubobject<UDemoAttributeSet>(TEXT("AttributeSet"));
}

void ADemoPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADemoPlayerState,Level); //TODO这个宏和其他的有什么区别？
	DOREPLIFETIME(ADemoPlayerState,XP);
	DOREPLIFETIME(ADemoPlayerState,CharacterClass);
	DOREPLIFETIME(ADemoPlayerState,LevelPoints);
	DOREPLIFETIME(ADemoPlayerState,Team);
}

UAbilitySystemComponent* ADemoPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ADemoPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void ADemoPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ADemoPlayerState::AddXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ADemoPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ADemoPlayerState::AddLevel(int32 InLevel)
{
	Level += InLevel;
	//升级要更新GE
	if(IsValid(GetPawn()))
	{
		ADemoPlayerCharacter* Character = Cast<ADemoPlayerCharacter>(GetPawn());
		Character->InitialDefaultAttributes();
	}
	OnLevelChangedDelegate.Broadcast(Level);
}

void ADemoPlayerState::AddLevelPoints(int32 InLevelPoints)
{
	LevelPoints += InLevelPoints;
	//TODO:在HUD中显示
}

void ADemoPlayerState::AddAbilityFromTagToPlayerCharacter(FGameplayTag InputActionTag)
{
	if(!HasAuthority())
	{
		return;
	}
	UDemoAbilitySystemComponent* DemoAbilitySystemComponent = Cast<UDemoAbilitySystemComponent>(AbilitySystemComponent);
	ADemoGameModeBase* DemoGameModeBase = Cast<ADemoGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(DemoGameModeBase)
	{
		FCharacterClassDefaultInfo CharacterClassInfo = DemoGameModeBase->CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
		for(TSubclassOf<UGameplayAbility> GAClass : CharacterClassInfo.AbilitiesClass)
		{
			//TODO: 问题：在试图判断能够拥有的技能的TriggerInputTag和我想要获取的技能的InputActionTag时，出现崩溃
			//判断 ： 由于GAClass是一个TSubclassOf,他只是一个类的模板信息，直接用它访问类的成员是不合法的
			//解决 ： 可以使用CDO或者创建实例来访问，但是由于这里需要判断每个GA的各自的成员值，不能用CDO(只能获取默认值)
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GAClass,1);
			UDemoGameplayAbilityBase* DemoGA = Cast<UDemoGameplayAbilityBase>(AbilitySpec.Ability);
			if(DemoGA->TriggerInputTag == InputActionTag)
			{
				DemoAbilitySystemComponent->AddAbilitiesToCharacter({GAClass});
			}
		}
	}
}

void ADemoPlayerState::SeamlessTravelTo(class APlayerState* NewPlayerState)
{
	Super::SeamlessTravelTo(NewPlayerState);
	if(ADemoPlayerState* NewPS = Cast<ADemoPlayerState>(NewPlayerState))
	{
		NewPS->Team = Team;
	}
}

void ADemoPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	if(ADemoPlayerState* NewPS = Cast<ADemoPlayerState>(PlayerState))
	{
		NewPS->Team = Team;
	}
}

void ADemoPlayerState::TryAddAbilityLevel_Implementation(FGameplayTag InputActionTag)
{
	bool bHasAbility = false;
	for(FGameplayAbilitySpec& AbilitySpec : AbilitySystemComponent->GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputActionTag))
		{
			if(AbilitySpec.Level < 5 && LevelPoints > 0)
			{
				AbilitySpec.Level++;
				UE_LOG(LogTemp,Display,TEXT("AbilityLevel: %d"),AbilitySpec.Level);
				bHasAbility = true;
				LevelPoints--;
			}
		}
	}
	//没有获取过能力
	if(!bHasAbility)
	{
		if(LevelPoints > 0)
		{
			AddAbilityFromTagToPlayerCharacter(InputActionTag);
			LevelPoints--;
		}
	}
}

void ADemoPlayerState::OnRep_Level(int32 OldLevel)
{
	//由于Set和Add函数只在Server调用，所以这个委托需要有OnRep函数在Client广播
	OnLevelChangedDelegate.Broadcast(Level);
}

void ADemoPlayerState::OnRep_LevelPoints()
{
	//TODO:在HUD中显示
}

void ADemoPlayerState::OnRep_Team()
{
	
}

void ADemoPlayerState::OnRep_XP()
{
	//由于Set和Add函数只在Server调用，所以这个委托需要有OnRep函数在Client广播
	OnXPChangedDelegate.Broadcast(XP);
}
