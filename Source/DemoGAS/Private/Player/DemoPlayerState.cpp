// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DemoPlayerState.h"

#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "GameplayAbilities/DemoAttributeSet.h"

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
	AttributeSet = CreateDefaultSubobject<UDemoAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ADemoPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ADemoPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
