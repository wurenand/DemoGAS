// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

struct FGameplayTag;

UINTERFACE(BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 战斗接口
 */
class DEMOGAS_API ICombatInterface
{
	GENERATED_BODY()

public:
	//返回角色等级
	virtual int32 GetPlayerLevel();
	//获取施法(Projectile)位置  
	//由继承者实现，根据InputTag,返回施法的位置，一般为继承者Weapon或Mesh上的某个Socket
	virtual FVector GetCombatSocketLocation(const FGameplayTag& InputTag);

	//利用MotionWarping插件实现转向，这个是执行转向的接口
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);

	//得到眩晕的蒙太奇动画
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetStunnedMontage();
};
