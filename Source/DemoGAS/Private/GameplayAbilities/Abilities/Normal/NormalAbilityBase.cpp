// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Abilities/Normal/NormalAbilityBase.h"

#include "GameplayTagsManager.h"

UNormalAbilityBase::UNormalAbilityBase()
{
	TriggerInputTag = UGameplayTagsManager::Get().RequestGameplayTag("InputAction.RMB");
}
