// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/DemoAbilitySystemComponent.h"


void UDemoAbilitySystemComponent::AfterInitialASCActorInfo()
{
	if(bAfterInitASCActorInfo) return;
	//确保只做一次
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UDemoAbilitySystemComponent::OnMyGameplayEffectAppliedToSelf);
	bAfterInitASCActorInfo = true;
}

void UDemoAbilitySystemComponent::AddAbilitiesToCharacter(const TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAdd)
{
	for(TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToAdd)
	{
		//在Server授予能力后，创建Spec并且Spec还会被复制到Client
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UDemoAbilitySystemComponent::OnMyGameplayEffectAppliedToSelf(UAbilitySystemComponent* ASC,
                                                                  const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer GameplayTags;
	GESpec.GetAllAssetTags(GameplayTags);
	OnAppliedGEToSelfAssetTagsDelegate.Broadcast(GameplayTags);
}

void UDemoAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	AfterInitialASCActorInfo();
}
