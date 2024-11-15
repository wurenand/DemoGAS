// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DemoRoomPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Game/DemoRoomGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "UI/Widget/DemoUserWidget.h"

void ADemoRoomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//非本地不要添加Widget
	if (!IsLocalController())
	{
		return;
	}
	//SpawnUI
	//直接让GameState作为UI的Controller
	checkf(RoomWidgetClass, TEXT("RoomWidgetClass is NULL"));
	RoomWidget = CreateWidget(this, RoomWidgetClass);
	UDemoUserWidget* DemoUserWidget = Cast<UDemoUserWidget>(RoomWidget);
	checkf(DemoUserWidget, TEXT("RoomWidgetClass Should be child class of UDemoUserWidget"));
	//让GameState作为RoomWidget的WidgetController 实现Room部分角色 阵营 英雄的UI同步！
	DemoUserWidget->SetWidgetController(GetWorld()->GetGameState());
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
