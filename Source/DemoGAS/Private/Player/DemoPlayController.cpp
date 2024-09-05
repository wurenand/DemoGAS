// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGAS/Public/Player/DemoPlayController.h"

#include "EnhancedInputSubsystems.h"


ADemoPlayController::ADemoPlayController()
{
	//TODO(为什么需要复制？)
	bReplicates = true;
}

void ADemoPlayController::BeginPlay()
{
	Super::BeginPlay();

	//添加输入映射上下文
	//断言？ 如果为空，会直接崩溃
	check(DefaultMappingContent)
	//增强输入本地玩家子系统是单例的，程序运行期间只存在一个
	UEnhancedInputLocalPlayerSubsystem* EILPSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EILPSubsystem)
	EILPSubsystem->AddMappingContext(DefaultMappingContent,0);

	//进行输入基本设置
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}
