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
	//PostLogin之后，记录连接了的PC CalledOnServer CalledOnServer
	void LoginPlayer(APlayerController* PlayerController);
	UFUNCTION(NetMulticast, reliable)
	void SetPlayerIsReady(APlayerController* PlayerController,bool bIsReady);

protected:
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Players,Category = "RoomGameState")
	TArray<APlayerController*> Players;

	UFUNCTION()
	void OnRep_Players(const TArray<APlayerController*>& OldPlayers);

	//用来记录玩家是否准备了， 不复制
	UPROPERTY(BlueprintReadOnly,Category = "RoomGameState")
	TMap<APlayerController*,bool> PlayerReadyState;

	//游戏地图的URL
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "RoomGameState")
	FString GameMapURL;
};
