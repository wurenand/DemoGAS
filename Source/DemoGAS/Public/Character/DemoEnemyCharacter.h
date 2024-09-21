// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoCharacterBase.h"
#include "Interface/InteractInterface.h"
#include "DemoEnemyCharacter.generated.h"

UCLASS()
class DEMOGAS_API ADemoEnemyCharacter : public ADemoCharacterBase
{
	GENERATED_BODY()

public:
	ADemoEnemyCharacter();
	

	//~Begin CombatInterface
	virtual int32 GetPlayerLevel() override;
	//~End

protected:
	virtual void BeginPlay() override;

	virtual void InitialAbilitySystem() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "CharacterDefault")
	int32 Level = 1; //AIController只存在于Server，所以不需要复制
};
