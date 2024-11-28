// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemoRoomGameMode.h"

#include "Game/DemoRoomGameState.h"
#include "Kismet/GameplayStatics.h"
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
}

void ADemoRoomGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//使用懒汉模式
	if (RoomGameState == nullptr)
	{
		RoomGameState = Cast<ADemoRoomGameState>(GetWorld()->GetGameState());
	}
	if (RoomGameState && HasAuthority())
	{
		//TODO:还有问题 本地应该只有自己的PlayerController？
		//还有问题: 如果后续有人登陆，在登陆之前的PlayerStateReady是不会同步的
		//解决方法:用OnRep_Players中添加Map映射
		RoomGameState->LoginPlayer(NewPlayer->PlayerState);
	}
}

void ADemoRoomGameMode::SelectTeam(APlayerController* PlayerController, ETeam InTeam)
{
	if (ADemoPlayerState* DemoPlayerState = PlayerController->GetPlayerState<ADemoPlayerState>())
	{
		//相同就不改了
		if (DemoPlayerState->Team != InTeam)
		{
			DemoPlayerState->Team = InTeam;
			//让GS多播更新UI
			RoomGameState->UpdatePlayerTeam(DemoPlayerState, InTeam);
		}
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
	RoomGameState->SetPlayerIsReady(PlayerController->PlayerState, bIsReady);
}
