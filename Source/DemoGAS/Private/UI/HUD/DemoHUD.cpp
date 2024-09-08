// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/DemoHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/DemoUserWidget.h"
#include "UI/WidgetController/DemoWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void ADemoHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("未在BP_DemoHUD中指定OverlayWidgetClass"))
	checkf(OverlayWidgetControllerClass,TEXT("未在BP_DemoHUD中指定OverlayWidgetControllerClass"))
	//创建OverlayWidget和Controller
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	UDemoWidgetController* WidgetController = NewObject<UDemoWidgetController>(this,OverlayWidgetControllerClass);
	//转型
	OverlayWidgetController = CastChecked<UOverlayWidgetController>(WidgetController);
	OverlayWidget = CastChecked<UDemoUserWidget>(Widget);
	//初始化MVC相应之间的联系
	FWidgetControllerParams Params(PC,PS,ASC,AS);
	OverlayWidgetController->SetWidgetControllerParams(Params);
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	
	OverlayWidget->AddToViewport();
}
