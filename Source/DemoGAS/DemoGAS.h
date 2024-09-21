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