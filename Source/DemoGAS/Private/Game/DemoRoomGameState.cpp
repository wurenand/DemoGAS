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
			SpawnPlayerDelegate.Broadcast(PS, 0);
		}
	}
}

void ADemoRoomGameState::SetPlayerIsReady_Implementation(APlayerState* PlayerState, bool bIsReady)
{
	UE_LOG(LogTemp, Display, TEXT("CheckReady_Next %d"),bIsReady);
	UE_LOG(LogTemp, Display, TEXT("CheckReady_MapValue %d"),PlayerReadyState[PlayerState]);
	if (Players.Contains(PlayerState) && PlayerReadyState[PlayerState] != bIsReady)
	{
		UE_LOG(LogTemp, Display, TEXT("CheckReady_NotEqual %d"),bIsReady);
		PlayerReadyState[PlayerState] = bIsReady;
		UpdateReadyDelegate.Broadcast(PlayerState, bIsReady ? 1 : 0);
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
