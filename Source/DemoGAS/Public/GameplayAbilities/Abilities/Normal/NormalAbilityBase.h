// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/Abilities/DamageGameplayAbility.h"
#include "NormalAbilityBase.generated.h"

/**
 * 普通攻击的GA的基类
 * 包括 攻击距离检测，InputTag
 */
UCLASS()
class DEMOGAS_API UNormalAbilityBase : public UDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UNormalAbilityBase();
};
