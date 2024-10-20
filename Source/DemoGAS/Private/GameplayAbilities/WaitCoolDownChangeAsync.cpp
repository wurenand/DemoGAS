// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/WaitCoolDownChangeAsync.h"

#include "AbilitySystemComponent.h"

UWaitCoolDownChangeAsync* UWaitCoolDownChangeAsync::WaitCoolDownChange(UAbilitySystemComponent* InASC,
                                                                       const FGameplayTag& InCoolDownTag)
{
	UWaitCoolDownChangeAsync* WaitCoolDownChangeAsync = NewObject<UWaitCoolDownChangeAsync>();
	WaitCoolDownChangeAsync->ASC = InASC;
	WaitCoolDownChangeAsync->CoolDownTag = InCoolDownTag;
	if (!IsValid(InASC) || !InCoolDownTag.IsValid())
	{
		WaitCoolDownChangeAsync->EndTask();
		return nullptr;
	}

	InASC->RegisterGameplayTagEvent(InCoolDownTag, EGameplayTagEventType::NewOrRemoved)
	     .AddUObject(WaitCoolDownChangeAsync, &UWaitCoolDownChangeAsync::CoolDownChanged);

	//在Server或者Client如果Apply了DurationGE就会广播
	InASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCoolDownChangeAsync,
	                                                            &UWaitCoolDownChangeAsync::GameplayEffectAppliedToSelf);

	return WaitCoolDownChangeAsync;
}

void UWaitCoolDownChangeAsync::EndTask()
{
	if (!IsValid(ASC))
	{
		return;
	}
	ASC->RegisterGameplayTagEvent(CoolDownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCoolDownChangeAsync::CoolDownChanged(const FGameplayTag InCoolDownTag, int NewCount)
{
	if (NewCount == 0)
	{
		OnCoolDownEnd.Broadcast(0.f);
	}
}

void UWaitCoolDownChangeAsync::GameplayEffectAppliedToSelf(UAbilitySystemComponent* TargetASC,
                                                           const FGameplayEffectSpec& EffectSpec,
                                                           FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CoolDownTag) || GrantedTags.HasTagExact(CoolDownTag))
	{
		//OwningTagsQuery = AssetTags + GrantedTags + DynamicGrantedTags
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
			CoolDownTag.GetSingleTagContainer());
		OnCoolDownStart.Broadcast(ASC->GetActiveEffectsTimeRemaining(Query)[0]);
	}
}
