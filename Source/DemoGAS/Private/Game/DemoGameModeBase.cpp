// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemoGameModeBase.h"

#include "Character/DemoPlayerCharacter.h"
#include "Player/DemoPlayerController.h"
#include "Player/DemoPlayerState.h"


void ADemoGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
	bFromSeamless = true;
	//HandleSeamlessTravelPlayer会调用RestartPlayer(Seamless时PostLogin不会调用，不会导致重复Spawn)
	Super::HandleSeamlessTravelPlayer(C);
}

void ADemoGameModeBase::RestartPlayer(AController* NewPlayer)
{
	if(bFromSeamless)
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
				//因为Collision问题导致失败
				//TODO:后续根据PlayerStart来进行生成
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
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
