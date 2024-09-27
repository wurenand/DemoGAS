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
	* 生成投射物1个 默认根据ICombatInterface获取生成位置  (高度将不会生效，设定为50？)
	 * @param TargetLocation 发射的目标位置
	 * @param bUseOverrideSpawnLoc 需要传入生成Trans时设置其为true，否则使用ICombatInterface中根据InputTag获取的Trans
	 * @param OverrideSpawnLocation 重载的生成位置，不使用的话可以随意传入
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnOneProjectile(FVector TargetLocation,FVector OverrideSpawnLocation,bool bUseOverrideSpawnLoc = false);
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	//这个GA生成的投射物的Class
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<ADemoProjectile> ProjectileClass;

	//投射物造成伤害的Effect
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
