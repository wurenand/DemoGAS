// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DemoPlayerState.h"

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
}

UAbilitySystemComponent* ADemoPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ADemoPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void ADemoPlayerState::OnRep_Level(int32 OldLevel)
{
	
}
