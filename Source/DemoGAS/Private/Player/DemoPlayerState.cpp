// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DemoPlayerState.h"

#include "Character/DemoPlayerCharacter.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "GameplayAbilities/DemoAttributeSet.h"
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
