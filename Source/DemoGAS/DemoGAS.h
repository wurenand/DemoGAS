// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//自定义宏变量
#define CUSTOM_DEPTH_RED 0.f
#define CUSTOM_DEPTH_WHITE 255.f

//阵营定义
UENUM(BlueprintType)
enum class ETeam : uint8
{
	ETeam_Red UMETA(DisplayName = "Red"),
	ETeam_Blue UMETA(DisplayName = "Blue"),
};

//自定义的Ability通道，重命名一下方便使用
#define ECC_Ability ECollisionChannel::ECC_GameTraceChannel1