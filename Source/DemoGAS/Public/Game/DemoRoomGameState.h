// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DemoRoomGameState.generated.h"

/**
 * Room模式的GS，用于保持角色的选择信息(Hero，Team) 已经当前房间的加入状态和准备状态
 */
UCLASS()
class DEMOGAS_API ADemoRoomGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Players,Category = "RoomGameState")
	TArray<APlayerController*> Players;

	UFUNCTION()
	void OnRep_Players();
};
