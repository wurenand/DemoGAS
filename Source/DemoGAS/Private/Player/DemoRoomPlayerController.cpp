// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DemoRoomPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Game/DemoRoomGameMode.h"

void ADemoRoomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//非本地不要添加Widget
	if (!IsLocalController())
	{
		return;
	}
	checkf(RoomWidgetClass, TEXT("RoomWidgetClass is NULL"));
	RoomWidget = CreateWidget(this, RoomWidgetClass);
	bShowMouseCursor = true;
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	RoomWidget->AddToViewport(0);
}

void ADemoRoomPlayerController::SetReady_Implementation(bool bIsReady)
{
	if (ADemoRoomGameMode* RoomGameMode = Cast<ADemoRoomGameMode>(GetWorld()->GetAuthGameMode()))
	{
		RoomGameMode->SetPlayerReady(this, bIsReady);
	}
}

void ADemoRoomPlayerController::SelectTeam_Implementation(ETeam InTeam)
{
	if (ADemoRoomGameMode* RoomGameMode = Cast<ADemoRoomGameMode>(GetWorld()->GetAuthGameMode()))
	{
		RoomGameMode->SelectTeam(this, InTeam);
	}
}

void ADemoRoomPlayerController::SelectHero_Implementation(ECharacterClass InCharacterClass)
{
	if (ADemoRoomGameMode* RoomGameMode = Cast<ADemoRoomGameMode>(GetWorld()->GetAuthGameMode()))
	{
		RoomGameMode->SelectHero(this, InCharacterClass);
	}
}
