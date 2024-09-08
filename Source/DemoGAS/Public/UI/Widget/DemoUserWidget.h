// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DemoUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMOGAS_API UDemoUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//自己的WidgetController，从此得到数据的广播信息，或者通知WidgetController事件
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* NewWidgetController);

	//在蓝图中实现，在WidgetController被设置后调用，绑定函数到WidgetController中的Delegate来更新数据等
	UFUNCTION(BlueprintImplementableEvent)
	void AfterWidgetControllerSet();
};
