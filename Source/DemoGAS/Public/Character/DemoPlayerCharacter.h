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

	//只会在Server调用
	virtual void PossessedBy(AController* NewController) override;
	//默认只在Client调用 并且一般只调用一次（变化后才会再次调用）
	virtual void OnRep_PlayerState() override;

private:
	UPROPERTY(EditAnywhere,Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(EditAnywhere,Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;

	void InitialASCActorInfo();
};
