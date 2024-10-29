// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemoRoomGameMode.h"

ADemoRoomGameMode::ADemoRoomGameMode()
{
	bUseSeamlessTravel = true;
}

void ADemoRoomGameMode::ServerTravel(const FString& URL,bool bAbsolute)
{
	if(!HasAuthority()) return;
	if(CanServerTravel(URL,bAbsolute))
	{
		GetWorld()->ServerTravel(URL,bAbsolute);
	}
}
