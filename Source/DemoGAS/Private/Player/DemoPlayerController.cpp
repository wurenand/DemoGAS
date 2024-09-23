﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGAS/Public/Player/DemoPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsManager.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "Input/DemoInputComponent.h"
#include "Interface/InteractInterface.h"
#include "Player/DemoPlayerState.h"


ADemoPlayerController::ADemoPlayerController()
{
	//TODO(为什么需要复制？)
	bReplicates = true;

	ClickMovePathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("ClickMovePathSpline"));
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

	DemoInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressesd,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagTriggered);
}


void ADemoPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	//执行鼠标指针射线检测
	GetHitResultUnderCursor(ECC_Visibility, false, CurrentCursorHit);
	//高亮
	TryHighlight();
	//右键移动
	TryAutoRun();
}


void ADemoPlayerController::TryHighlight()
{
	if(!CurrentCursorHit.bBlockingHit)
	{
		return;
	}
	//更新上一帧和当前帧
	LastFrameActor = CurrentFrameActor;
	CurrentFrameActor = Cast<IInteractInterface>(CurrentCursorHit.GetActor());

	//记录是否悬停在某个Actor身上
	if(CurrentFrameActor != nullptr)
	{
		bIsTargeting = true;
	}
	else
	{
		bIsTargeting = false;
	}
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
		CurrentFrameActor->HighlightActor(GetPlayerState<ADemoPlayerState>()->Team);
	}
	else
	{
		//B
		if(LastFrameActor == nullptr)
		{
			CurrentFrameActor->HighlightActor(GetPlayerState<ADemoPlayerState>()->Team);
			return;
		}
		//C
		LastFrameActor->UnHighlightActor();
	}
}


void ADemoPlayerController::TryAutoRun()
{
	if(!bAutoRunning) return;
	
	//当控制的Pawn存在时（避免角色死亡后还调用）
	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = ClickMovePathSpline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		//获得样条线上点的切线方向 ？
		const FVector DirectionOnSpline = ClickMovePathSpline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(DirectionOnSpline);

		const float DistanceToDestination = (CachedDestination - LocationOnSpline).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}


void ADemoPlayerController::TryMoveDestination()
{
	//1 获取当前帧碰撞点
	if(GetHitResultUnderCursor(ECC_Visibility, false, CurrentCursorHit))
	{
		CachedDestination = CurrentCursorHit.ImpactPoint;
	}
	if(APawn* ControlledPawn = GetPawn())
	{
		//2 利用Navigation系统来寻找路径
		UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
			this, ControlledPawn->GetActorLocation(), CachedDestination);
		//3 如果找到就往 样条线组件中添加路径点，实现平滑路径移动
		if(Path)
		{
			ClickMovePathSpline->ClearSplinePoints();
			for(const FVector& Point : Path->PathPoints)
			{
				ClickMovePathSpline->AddSplinePoint(Point,ESplineCoordinateSpace::World);
				//绘制一下调试球
				DrawDebugSphere(GetWorld(),Point,5.f,8,FColor::Green);
			}
			bAutoRunning = true;
		}
		else
		{
			bAutoRunning = false;
		}
	}
}



void ADemoPlayerController::AbilityInputTagPressesd(FGameplayTag InputTag)
{
	//TODO:没完成
	if(!GetASC())
	{
		return;
	}
}

void ADemoPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(!GetASC())
	{
		return;
	}
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ADemoPlayerController::AbilityInputTagTriggered(FGameplayTag InputTag)
{
	//鼠标右键逻辑 移动 平a
	if(InputTag.MatchesTagExact(UGameplayTagsManager::Get().RequestGameplayTag(FName(TEXT("InputAction.RMB")))))
	{
		if(bIsTargeting)
		{
			//检测是否在攻击范围内，如果在，直接攻击， 不在就先移动再攻击
		}
		else
		{
			//移动
			TryMoveDestination();
		}
	}

	if(!GetASC())
	{
		return;
	}
	GetASC()->AbilityInputTagTriggered(InputTag);
}



UDemoAbilitySystemComponent* ADemoPlayerController::GetASC()
{
	//确保只进行一次Cast
	if(DemoASC == nullptr)
	{
		DemoASC = Cast<UDemoAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return DemoASC;
}
