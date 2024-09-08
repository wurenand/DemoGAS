// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/DemoWidgetController.h"

void UDemoWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}
