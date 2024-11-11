// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoRoomGameMode.generated.h"

class ADemoRoomGameState;
enum class ECharacterClass : uint8;
enum class ETeam : uint8;
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

	virtual void BeginPlay() override;
	
	//将玩家注册到GameState中，用于管理对局是否开始等
	virtual void PostLogin(APlayerController* NewPlayer) override;

	//TODO:相应玩家交互的事件
	UFUNCTION(BlueprintCallable)
	void SelectTeam(APlayerController* PlayerController,ETeam InTeam);
	UFUNCTION(BlueprintCallable)
	void SelectHero(APlayerController* PlayerController,ECharacterClass InCharacterClass);
	UFUNCTION(BlueprintCallable)
	void SetPlayerReady(APlayerController* PlayerController,bool bIsReady);

private:
	TObjectPtr<ADemoRoomGameState> RoomGameState;
};
