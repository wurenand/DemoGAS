// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/DemoAbilitySystemComponent.h"

#include "GameplayAbilities/Abilities/DemoGameplayAbilityBase.h"


void UDemoAbilitySystemComponent::AfterInitialASCActorInfo()
{
	if(bAfterInitASCActorInfo) return;
	//确保只做一次
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UDemoAbilitySystemComponent::ClientOnMyGameplayEffectAppliedToSelf);
	bAfterInitASCActorInfo = true;
}

void UDemoAbilitySystemComponent::AddAbilitiesToCharacter(const TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAdd)
{
	for(TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToAdd)
	{
		//在Server授予能力后，创建Spec并且Spec还会被复制到Client
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);

		//添加StartUpTag
		if(const UDemoGameplayAbilityBase* DemoGameplayAbility = Cast<UDemoGameplayAbilityBase>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(DemoGameplayAbility->TriggerInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UDemoAbilitySystemComponent::AbilityInputTagTriggered(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);//需要自己实现功能，只是通知Ability被Pressed了
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UDemoAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);//需要自己实现功能，只是通知Ability被Releassed了
		}
	}
}

void UDemoAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	//TODO:没完成
	if(!InputTag.IsValid()) return;
}

void UDemoAbilitySystemComponent::ClientOnMyGameplayEffectAppliedToSelf_Implementation(UAbilitySystemComponent* ASC,
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
