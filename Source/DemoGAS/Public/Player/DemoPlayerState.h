// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DemoPlayerState.generated.h"

/**
 * 在PlayerState中给玩家添加AbilitySystemComponent和AttributeSet
 * 这样可以在玩家死亡时依然保留ASC和属性集
 * 但是对于AI，可以直接在Character上创建ASC和属性集
 */
UCLASS()
class DEMOGAS_API ADemoPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADemoPlayerState();
};
