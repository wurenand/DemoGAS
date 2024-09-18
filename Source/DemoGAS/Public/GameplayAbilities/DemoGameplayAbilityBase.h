// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DemoGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class DEMOGAS_API UDemoGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	//触发这个Ability需要的InputTag
	UPROPERTY(EditDefaultsOnly,Category = "Input")
	FGameplayTag TriggerInputTag;
};
