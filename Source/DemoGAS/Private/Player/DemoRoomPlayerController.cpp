// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DemoRoomPlayerController.h"

#include "Blueprint/UserWidget.h"

void ADemoRoomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//非本地不要添加Widget
	if(!IsLocalController()) return;
	checkf(RoomWidgetClass,TEXT("RoomWidgetClass is NULL"));
	RoomWidget = CreateWidget(this,RoomWidgetClass);
	bShowMouseCursor = true;
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	RoomWidget->AddToViewport(0);
}
