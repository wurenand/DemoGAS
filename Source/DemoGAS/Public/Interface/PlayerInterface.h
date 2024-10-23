// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 处理与玩家角色相关的逻辑
 * 升级 增加XP等
 */
class DEMOGAS_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	void AddXP(int32 InXP);
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP();
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelFromXP(int32 XP);
	//有什么用？
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
	UFUNCTION(BlueprintNativeEvent)
	void AddPlayerLevel(int32 LevelToAdd);
};
