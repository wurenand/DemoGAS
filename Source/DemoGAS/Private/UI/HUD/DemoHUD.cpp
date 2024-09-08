// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/DemoHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/DemoUserWidget.h"

void ADemoHUD::BeginPlay()
{
	Super::BeginPlay();

	//创建OverlayWidget
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();

	OverlayWidget = Cast<UDemoUserWidget>(Widget);
}
