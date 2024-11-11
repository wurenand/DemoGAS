// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemoRoomGameState.h"

#include "Game/DemoRoomGameMode.h"
#include "Net/UnrealNetwork.h"

void ADemoRoomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADemoRoomGameState, Players);
}

void ADemoRoomGameState::SetPlayerIsReady_Implementation(APlayerController* PlayerController, bool bIsReady)
{
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

void ADemoRoomGameState::LoginPlayer_Implementation(APlayerController* PlayerController)
{
	Players.Add(PlayerController);
	//TODO:用一个委托，通知UI生成图标
	PlayerReadyState.Add(PlayerController, false);
}
