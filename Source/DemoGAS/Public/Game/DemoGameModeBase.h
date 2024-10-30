// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoGameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class DEMOGAS_API ADemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,Category = "CharacterClassDefaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	virtual void RestartPlayer(AController* NewPlayer) override;
};
