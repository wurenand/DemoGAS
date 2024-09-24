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

public:
	/**
	 * 生成投射物1个 默认根据ICombatInterface获取生成位置
	 * @param bUseOverrideTrans 需要传入生成Trans时设置其为true，否则使用ICombatInterface中根据InputTag获取的Trans
	 * @param OverrideTransform 覆盖的新Trans
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnOneProjectile(bool bUseOverrideTrans = false, FTransform OverrideTransform = FTransform());
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	//这个GA生成的投射物的Class
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<ADemoProjectile> ProjectileClass;
};
