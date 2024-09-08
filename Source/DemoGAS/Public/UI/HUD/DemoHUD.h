// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DemoHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UDemoWidgetController;
class UDemoUserWidget;
/**
 * 创建管理OverlayWidget（MainUI）
 */
UCLASS()
class DEMOGAS_API ADemoHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UDemoUserWidget> OverlayWidget;
	UPROPERTY()
	TObjectPtr<UDemoWidgetController> OverlayWidgetController;

	//在PlayerCharacter中的InitialActorInfo中调用最好
	void InitOverlay(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS);

protected:
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDemoWidgetController> OverlayWidgetControllerClass;
};
