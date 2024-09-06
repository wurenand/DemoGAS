// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGAS/Public/Player/DemoPlayController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interface/EnemyInterface.h"


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
	UEnhancedInputLocalPlayerSubsystem* EILPSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(EILPSubsystem)
	EILPSubsystem->AddMappingContext(DefaultMappingContent, 0);

	//进行输入基本设置
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ADemoPlayController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//CastCheck相当于Cast之后使用了check断言，InValid的时候会Crash
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADemoPlayController::Move);
}

void ADemoPlayController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	//执行鼠标指针射线检测，高亮
	CursorTrace();
}

void ADemoPlayController::CursorTrace()
{
	FHitResult CursorResult;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorResult);
	if(!CursorResult.bBlockingHit) return;
	//更新上一帧和当前帧
	LastFrameActor = CurrentFrameActor;
	CurrentFrameActor = Cast<IEnemyInterface>(CursorResult.GetActor());
	/*
	 *	   Last Current
	 * A	n		n	DoNothing
	 * B	n		Y	Highlight Current
	 * C	Y		n	UnHighlight Last
	 * D	Y	!=	Y	Unhighlight Last Highlight Current
	 * E	Y   ==	Y   DoNothing
	 */
	if(LastFrameActor == nullptr && CurrentFrameActor == nullptr)
	{
		//A
		if(LastFrameActor == nullptr) return;
		//E
		if(LastFrameActor == CurrentFrameActor) return;
		//D
		LastFrameActor->UnHighlightActor();
		CurrentFrameActor->HighlightActor();
	}
	else
	{
		//B
		if(LastFrameActor == nullptr)
		{
			CurrentFrameActor->HighlightActor();
			return;
		}
		//C
		LastFrameActor->UnHighlightActor();
	}
	
}


void ADemoPlayController::Move(const FInputActionValue& Value)
{
	const FVector2d InputAxisValue = Value.Get<FVector2d>();

	const FRotator RotationYaw = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	//利用旋转矩阵，并拿到旋转矩阵的不同方向的值再归一化得到方向
	const FVector ForwardDirection = FRotationMatrix(RotationYaw).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(RotationYaw).GetUnitAxis(EAxis::Y);

	//因为Move可能在Pawn生成之前就被调用，所以这里不用check断言
	if(APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisValue.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisValue.X);
	}
}

