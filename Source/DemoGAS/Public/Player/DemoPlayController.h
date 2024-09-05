// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DemoPlayController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class DEMOGAS_API ADemoPlayController : public APlayerController
{
	GENERATED_BODY()

public:
	ADemoPlayController();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContent;//记得在Build.cs导入EnhancedInput模块
};
