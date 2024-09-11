// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/DemoAbilitySystemComponent.h"


void UDemoAbilitySystemComponent::AfterInitialASCActorInfo()
{
	if(bAfterInitASCActorInfo) return;
	//确保只做一次
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UDemoAbilitySystemComponent::OnMyGameplayEffectAppliedToSelf);
	bAfterInitASCActorInfo = true;
}

void UDemoAbilitySystemComponent::OnMyGameplayEffectAppliedToSelf(UAbilitySystemComponent* ASC,
                                                                  const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	UE_LOG(LogTemp,Warning,TEXT("ApplyGEToSelf"));
}

void UDemoAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	AfterInitialASCActorInfo();
}
