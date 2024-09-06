// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoCharacterBase.h"
#include "DemoPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class DEMOGAS_API ADemoPlayerCharacter : public ADemoCharacterBase
{
	GENERATED_BODY()

public:
	ADemoPlayerCharacter();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(EditAnywhere,Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
};
