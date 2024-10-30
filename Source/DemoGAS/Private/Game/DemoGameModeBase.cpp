﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemoGameModeBase.h"

#include "Character/DemoPlayerCharacter.h"
#include "Player/DemoPlayerController.h"
#include "Player/DemoPlayerState.h"

void ADemoGameModeBase::RestartPlayer(AController* NewPlayer)
{
	if(GetWorld()->IsInSeamlessTravel())
	{
		if (ADemoPlayerController* NewPlayerController = Cast<ADemoPlayerController>(NewPlayer))
		{
			if (ADemoPlayerState* DemoPlayerState = NewPlayerController->GetPlayerState<ADemoPlayerState>())
			{
				FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(
					DemoPlayerState->CharacterClass);
				checkf(CharacterClassDefaultInfo.PlayerCharacterBPClass,
					   TEXT("CharacterClassDefaultInfo.PlayerCharacterBPClass is null"));
				TSubclassOf<ADemoPlayerCharacter> SpawnClass = CharacterClassDefaultInfo.PlayerCharacterBPClass;
				FTransform SpawnTransform;
				FActorSpawnParameters SpawnParameters;
				ADemoPlayerCharacter* SpawnCharacter = GetWorld()->SpawnActor<ADemoPlayerCharacter>(SpawnClass,SpawnTransform,SpawnParameters);
				NewPlayerController->SetPawn(SpawnCharacter);
				FinishRestartPlayer(NewPlayerController,FRotator(0.0f));
			}
		}
	}
	else
	{
		Super::RestartPlayer(NewPlayer);
	}
}
