// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemoRoomGameState.h"

#include "Game/DemoRoomGameMode.h"
#include "Net/UnrealNetwork.h"

void ADemoRoomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADemoRoomGameState, Players);
}

void ADemoRoomGameState::OnRep_Players(const TArray<APlayerController*>& OldPlayers)
{
	for (APlayerController* PC : Players)
	{
		if (!OldPlayers.Contains(PC))
		{
			PlayerReadyState.Add(PC, false);
			UE_LOG(LogTemp,Warning,TEXT("ADemoRoomGameState::OnRep_Players"));
		}
	}
}

void ADemoRoomGameState::SetPlayerIsReady_Implementation(APlayerController* PlayerController, bool bIsReady)
{
	bool bIsServer = HasAuthority();
	static int32 count = 0;
	UE_LOG(LogTemp, Warning, TEXT("isServer %d  count : %d"), bIsServer, ++count);
	if (Players.Contains(PlayerController) && PlayerReadyState[PlayerController] != bIsReady)
	{
		PlayerReadyState[PlayerController] = bIsReady;
		//TODO:用一个委托通知UI改变状态
		//检查是否全部准备了
		if (HasAuthority())
		{
			bool bShouldStartGame = true;
			for (TTuple<APlayerController*, bool> ReadyState : PlayerReadyState)
			{
				if (!ReadyState.Value)
				{
					bShouldStartGame = false;
					break;
				}
			}
			if (bShouldStartGame)
			{
				ADemoRoomGameMode* DemoRoomGameMode = Cast<ADemoRoomGameMode>(GetWorld()->GetAuthGameMode());
				DemoRoomGameMode->ServerTravel(GameMapURL, true);
			}
		}
	}
}

void ADemoRoomGameState::LoginPlayer(APlayerController* PlayerController)
{
	TArray<APlayerController*> OldPlayers = Players;
	Players.Add(PlayerController);
	OnRep_Players(OldPlayers);
}


