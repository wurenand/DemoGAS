// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DemoPlayerController.generated.h"

class UDamageWidgetComponent;
class IInteractInterface;
class USplineComponent;
class UDemoAbilitySystemComponent;
struct FGameplayTag;
class UDemoInputConfig;
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

	//生成显示浮动伤害数字的组件 ClientRPC
	UFUNCTION(Client,Reliable) 
	void ShowDamageNumber(float DamageValue,bool bIsCriticalHit ,AActor* TargetActor);

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
	
	//检测鼠标指针有没有悬停到Enemy上
	void TryHighlight();
	bool bIsTargeting = false;
	//记录当前这一帧的鼠标悬停检测结果，供需要的功能使用
	FHitResult CurrentCursorHit;
	IInteractInterface* LastFrameActor;
	IInteractInterface* CurrentFrameActor;

	//~begin Input相应的三个回调函数
	//InputConfig中配置的所有IA都会在DemoInputComponent中被绑定到这三个函数，通过传入的Tag来区分按键
	void AbilityInputTagPressesd(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagTriggered(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UDemoInputConfig> InputConfig;
	//~end


	//~Begin ASC相关
	UDemoAbilitySystemComponent* GetASC();
	
	UPROPERTY()
	TObjectPtr<UDemoAbilitySystemComponent> DemoASC;
	//~End

	//~Begin 点击右键移动
	void TryMoveDestination();
	void TryAutoRun();
	FVector CachedDestination = FVector::ZeroVector;//目标点
	bool bAutoRunning = false;//当为true的时候，向目标点移动
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;//到达目标点可容忍接受度

	TObjectPtr<USplineComponent> ClickMovePathSpline; //使用样条线平滑移动路线
	//~End

	//显示浮动伤害的组件类
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageWidgetComponent> DamageWidgetComponentClass;
};
