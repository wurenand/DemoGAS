// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoCharacterBase.h"
#include "Interface/EnemyInterface.h"
#include "DemoEnemyCharacter.generated.h"

UCLASS()
class DEMOGAS_API ADemoEnemyCharacter : public ADemoCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()

public:
	ADemoEnemyCharacter();
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

protected:
	virtual void BeginPlay() override;

	virtual void InitialAbilitySystem() override;
};
