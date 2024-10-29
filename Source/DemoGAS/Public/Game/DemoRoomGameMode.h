// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoRoomGameMode.generated.h"

/**
 * 作为选择英雄的地图的GameMode，使用SeamlessTravel来保存选择信息
 * 封装ServerTravel给蓝图
 */
UCLASS()
class DEMOGAS_API ADemoRoomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADemoRoomGameMode();

	UFUNCTION(BlueprintCallable, Category="Map")
	void ServerTravel(const FString& URL,bool bAbsolute);
};
