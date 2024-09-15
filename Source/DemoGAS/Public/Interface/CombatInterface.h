// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

UINTERFACE()
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
};
