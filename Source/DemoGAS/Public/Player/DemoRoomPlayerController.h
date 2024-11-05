// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DemoRoomPlayerController.generated.h"

/**
 * 作为Room关卡中的PC，控制RoomUI
 */
UCLASS()
class DEMOGAS_API ADemoRoomPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	protected:
	UPROPERTY(EditAnywhere,Category = "RoomUI")
	TSubclassOf<UUserWidget> RoomWidgetClass;
	UPROPERTY(EditAnywhere,Category = "RoomUI")
	TObjectPtr<UUserWidget> RoomWidget;
	
};
