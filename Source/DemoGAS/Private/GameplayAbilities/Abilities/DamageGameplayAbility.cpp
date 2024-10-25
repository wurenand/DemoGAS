// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Abilities/DamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "Character/DemoCharacterBase.h"
#include "GameplayAbilities/DemoAttributeSet.h"

UDemoAttributeSet* UDamageGameplayAbility::GetSourceAttributeSet()
{
	return Cast<UDemoAttributeSet>(Cast<ADemoCharacterBase>(GetAvatarActorFromActorInfo())->GetAttributeSet());
}

void UDamageGameplayAbility::SetDamageValue(float InDamageValue)
{
	DamageValue = InDamageValue;
}

void UDamageGameplayAbility::CauseDamage(ADemoCharacterBase* InTargetActor)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), ContextHandle);
	const float ScaledDamage = DamageValue.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,
	                                                              UGameplayTagsManager::Get().RequestGameplayTag(
		                                                              FName("DamageCaller")), ScaledDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data, InTargetActor->GetAbilitySystemComponent());
}
