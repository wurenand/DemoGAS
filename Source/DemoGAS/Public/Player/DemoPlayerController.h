﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DemoPlayerController.generated.h"

struct FGameplayTag;
class UDemoInputConfig;
class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class DEMOGAS_API ADemoPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADemoPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	//用来处理输入逻辑
	//PlayerController 具有 PlayerInput 对象时，才会调用 PlayerTick。
	//因此，它只会对本地控制的 PlayerController 进行调用。(复制的不会)
	virtual void PlayerTick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContent;//记得在Build.cs导入EnhancedInput模块

	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& Value);

	//检测鼠标指针有没有悬停到Enemy上
	void CursorTrace();
	IEnemyInterface* LastFrameActor;
	IEnemyInterface* CurrentFrameActor;

	//~begin Input
	//InputConfig中配置的所有IA都会在DemoInputComponent中被绑定到这三个函数，通过传入的Tag来区分按键
	void AbilityInputTagPressesd(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagTriggered(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UDemoInputConfig> InputConfig;
	//~end
};
