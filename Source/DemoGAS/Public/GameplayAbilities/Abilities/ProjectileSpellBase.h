// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoGameplayAbilityBase.h"
#include "ProjectileSpellBase.generated.h"

class ADemoProjectile;
/**
 * 投射物能力类的基类
 */
UCLASS()
class DEMOGAS_API UProjectileSpellBase : public UDemoGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	//这个GA生成的投射物的Class
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<ADemoProjectile> ProjectileClass;
};
