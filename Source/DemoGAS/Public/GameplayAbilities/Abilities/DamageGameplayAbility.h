// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoGameplayAbilityBase.h"
#include "DamageGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Damage_Physics,
	Damage_Magic,
	Damage_True,
};
/**
 * 
 */
UCLASS()
class DEMOGAS_API UDamageGameplayAbility : public UDemoGameplayAbilityBase
{
	GENERATED_BODY()

public:
	EDamageType GetDamageType() const { return DamageType; }
	
protected:
	//投造成伤害的Effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	//伤害值
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	FScalableFloat DamageValue;
	//伤害类型
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Damage")
	EDamageType DamageType;
};
