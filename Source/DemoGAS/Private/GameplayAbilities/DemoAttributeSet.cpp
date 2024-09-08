// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/DemoAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UDemoAttributeSet::UDemoAttributeSet()
{
	//初始化Attribute
	InitHealth(50);
	InitMaxHealth(100);
	InitMana(100);
	InitMaxMana(100);
}

void UDemoAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,Health,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDemoAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
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
