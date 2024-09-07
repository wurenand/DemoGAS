// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/DemoUserWidget.h"

void UDemoUserWidget::SetWidgetController(UObject* NewWidgetController)
{
	WidgetController = NewWidgetController;
	AfterWidgetControllerSet();
}
