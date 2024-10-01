// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetPlayerLevel()
{
	//提供默认实现
	return 0;
}

FVector ICombatInterface::GetCombatSocketLocation(const FGameplayTag& InputTag)
{
	return FVector();
}