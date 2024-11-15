// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DemoRoomGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdatePlayerEnumSignature,APlayerController*,PC,uint8 ,EnumValue);

enum class ECharacterClass : uint8;
enum class ETeam : uint8;
/**
 * Room模式的GS，用于保持角色的选择信息(Hero，Team) 已经当前房间的加入状态和准备状态
 */
UCLASS()
class DEMOGAS_API ADemoRoomGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly,BlueprintAssignable,Category = "PlayerInfo")
	FUpdatePlayerEnumSignature UpdateTeamDelegate;
	UPROPERTY(BlueprintReadOnly,BlueprintAssignable,Category = "PlayerInfo")
	FUpdatePlayerEnumSignature UpdateHeroDelegate;
	UPROPERTY(BlueprintReadOnly,BlueprintAssignable,Category = "PlayerInfo")
	FUpdatePlayerEnumSignature UpdateReadyDelegate;
	UPROPERTY(BlueprintReadOnly,BlueprintAssignable,Category = "PlayerInfo")
	FUpdatePlayerEnumSignature SpawnPlayerDelegate;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//PostLogin之后，记录连接了的PC CalledOnServer
	void LoginPlayer(APlayerController* PlayerController);
	UFUNCTION(NetMulticast, reliable)
	void SetPlayerIsReady(APlayerController* PlayerController,bool bIsReady);
	//多播来使用委托同步到所有人的UI上
	UFUNCTION(NetMulticast, reliable)
	void UpdatePlayerTeam(APlayerController* PlayerController,ETeam NewTeam);
	//多播来使用委托同步到所有人的UI上
	UFUNCTION(NetMulticast, reliable)
	void UpdatePlayerIsHero(APlayerController* PlayerController,ECharacterClass NewCharacter);

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
