// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DemoPlayController.generated.h"

class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class DEMOGAS_API ADemoPlayController : public APlayerController
{
	GENERATED_BODY()

public:
	ADemoPlayController();

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
};
