// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGAS/Public/Player/DemoPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "Input/DemoInputComponent.h"
#include "Interface/EnemyInterface.h"


ADemoPlayerController::ADemoPlayerController()
{
	//TODO(为什么需要复制？)
	bReplicates = true;
}

void ADemoPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//添加输入映射上下文
	//断言？ 如果为空，会直接崩溃
	check(DefaultMappingContent)
	//增强输入本地玩家子系统是单例的，程序运行期间只存在一个
	UEnhancedInputLocalPlayerSubsystem* EILPSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(EILPSubsystem)
	{
		EILPSubsystem->AddMappingContext(DefaultMappingContent, 0);
	}
	//进行输入基本设置
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ADemoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//CastCheck相当于Cast之后使用了check断言，InValid的时候会Crash
	//使用自己的Component (记得在项目设置中设置)
	UDemoInputComponent* DemoInputComponent = CastChecked<UDemoInputComponent>(InputComponent);

	DemoInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADemoPlayerController::Move);

	DemoInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressesd,
	                                       &ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagTriggered);
}

void ADemoPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	//执行鼠标指针射线检测，高亮
	CursorTrace();
}

void ADemoPlayerController::CursorTrace()
{
	FHitResult CursorResult;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorResult);
	if(!CursorResult.bBlockingHit)
	{
		return;
	}
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
	if((LastFrameActor == nullptr) == (CurrentFrameActor == nullptr))
	{
		//A
		if(LastFrameActor == nullptr)
		{
			return;
		}
		//E
		if(LastFrameActor == CurrentFrameActor)
		{
			return;
		}
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

void ADemoPlayerController::AbilityInputTagPressesd(FGameplayTag InputTag)
{
	//TODO:没完成
	if(!GetASC()) return;
}

void ADemoPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!GetASC()) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ADemoPlayerController::AbilityInputTagTriggered(FGameplayTag InputTag)
{
	if(!GetASC()) return;
	GetASC()->AbilityInputTagTriggered(InputTag);
}

UDemoAbilitySystemComponent* ADemoPlayerController::GetASC()
{
	//确保只进行一次Cast
	if(DemoASC == nullptr)
	{
		DemoASC = Cast<UDemoAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return DemoASC;
}


void ADemoPlayerController::Move(const FInputActionValue& Value)
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
