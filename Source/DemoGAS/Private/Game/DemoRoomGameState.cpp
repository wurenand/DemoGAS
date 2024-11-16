// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemoRoomGameState.h"

#include "Game/DemoRoomGameMode.h"
#include "Net/UnrealNetwork.h"

void ADemoRoomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADemoRoomGameState, Players);
}

void ADemoRoomGameState::OnRep_Players(const TArray<APlayerState*>& OldPlayers)
{
	for (APlayerState* PS : Players)
	{
		if (!OldPlayers.Contains(PS))
		{
			PlayerReadyState.Add(PS, false);
			//TODO:不应该使用PlayerController 因为Client根本没有！ 只有自己的Controller
			//所以只传递这个指针却找不到具体的对象！  但是我需要找到这个对象吗？
			SpawnPlayerDelegate.Broadcast(PS, 0);
		}
	}
}

void ADemoRoomGameState::SetPlayerIsReady_Implementation(APlayerState* PlayerState, bool bIsReady)
{
	bool bIsServer = HasAuthority();
	static int32 count = 0;
	UE_LOG(LogTemp, Warning, TEXT("isServer %d  count : %d"), bIsServer, ++count);
	if (Players.Contains(PlayerState) && PlayerReadyState[PlayerState] != bIsReady)
	{
		PlayerReadyState[PlayerState] = bIsReady;
		UpdateReadyDelegate.Broadcast(PlayerState, bIsReady);
		//检查是否全部准备了
		if (HasAuthority())
		{
			bool bShouldStartGame = true;
			for (TTuple<APlayerState*, bool> ReadyState : PlayerReadyState)
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

void ADemoRoomGameState::LoginPlayer(APlayerState* PlayerController)
{
	TArray<APlayerState*> OldPlayers = Players;
	Players.Add(PlayerController);
	OnRep_Players(OldPlayers);
}

void ADemoRoomGameState::UpdatePlayerTeam_Implementation(APlayerState* PlayerState, ETeam NewTeam)
{
	UpdateTeamDelegate.Broadcast(PlayerState, static_cast<uint8>(NewTeam));
}

void ADemoRoomGameState::UpdatePlayerIsHero_Implementation(APlayerState* PlayerState,
                                                           ECharacterClass NewCharacter)
{
	UpdateHeroDelegate.Broadcast(PlayerState, static_cast<uint8>(NewCharacter));
}
