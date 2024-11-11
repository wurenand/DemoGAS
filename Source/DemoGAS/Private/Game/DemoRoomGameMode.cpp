// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemoRoomGameMode.h"

#include "Game/DemoRoomGameState.h"
#include "Player/DemoPlayerState.h"

ADemoRoomGameMode::ADemoRoomGameMode()
{
	bUseSeamlessTravel = true;
}

void ADemoRoomGameMode::ServerTravel(const FString& URL, bool bAbsolute)
{
	UE_LOG(LogTemp, Display, TEXT("MyServerTravel"));
	//TODO:SeamlessTravel失效？无法执行无缝转移
	if (!HasAuthority())
	{
		return;
	}
	if (CanServerTravel(URL, bAbsolute))
	{
		GetWorld()->ServerTravel(URL, bAbsolute);
	}
}

void ADemoRoomGameMode::BeginPlay()
{
	Super::BeginPlay();
	RoomGameState = Cast<ADemoRoomGameState>(GetGameState<ADemoRoomGameState>());
}

void ADemoRoomGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	RoomGameState->LoginPlayer(NewPlayer);
}

void ADemoRoomGameMode::SelectTeam(APlayerController* PlayerController, ETeam InTeam)
{
	if (ADemoPlayerState* DemoPlayerState = PlayerController->GetPlayerState<ADemoPlayerState>())
	{
		DemoPlayerState->Team = InTeam;
	}
}

void ADemoRoomGameMode::SelectHero(APlayerController* PlayerController, ECharacterClass InCharacterClass)
{
	if (ADemoPlayerState* DemoPlayerState = PlayerController->GetPlayerState<ADemoPlayerState>())
	{
		DemoPlayerState->CharacterClass = InCharacterClass;
	}
}

void ADemoRoomGameMode::SetPlayerReady(APlayerController* PlayerController, bool bIsReady)
{
}
