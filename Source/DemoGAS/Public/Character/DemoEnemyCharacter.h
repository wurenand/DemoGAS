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

	//~Begin EnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~End

	//~Begin CombatInterface
	virtual int32 GetPlayerLevel() override;
	//~End

protected:
	virtual void BeginPlay() override;

	virtual void InitialAbilitySystem() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "CharacterDefault")
	int32 Level = 1; //AIController只存在于Server，所以不需要复制
};
