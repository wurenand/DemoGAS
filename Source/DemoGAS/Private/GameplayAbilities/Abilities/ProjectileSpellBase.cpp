// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Abilities/ProjectileSpellBase.h"

#include "Kismet/KismetSystemLibrary.h"

void UProjectileSpellBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//TODO:为什么从Client激活会激活两次？
	UKismetSystemLibrary::PrintString(this,FString(TEXT("C++ 激活")));
}
