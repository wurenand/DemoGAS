// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/DemoAbilitySystemGlobals.h"

#include "DemoAbilitySystemTypes.h"

FGameplayEffectContext* UDemoAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FDemoGameplayEffectContext();
}
