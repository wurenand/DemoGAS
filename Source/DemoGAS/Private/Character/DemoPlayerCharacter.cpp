﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGAS/Public/Character/DemoPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayAbilities/Data/LevelUpInfo.h"
#include "GameplayAbilities/Library/DemoSystemLibrary.h"
#include "Player/DemoPlayerController.h"
#include "Player/DemoPlayerState.h"
#include "UI/HUD/DemoHUD.h"

ADemoPlayerCharacter::ADemoPlayerCharacter()
{
	//摄像机和弹簧臂基本设置
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 750.f;
	//禁止摄像机继承Character旋转
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraLag = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);


	//设置俯视角游戏的基本设置
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//TODO:需要阅读CharacterMovement源码，看看如何实现转向不减速
	GetCharacterMovement()->RotationRate = FRotator(0.f, 1000.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true; //移动会被约束到平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true; //开始时与平面对齐

	//避免摄像机视角影响角色视角
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

FVector ADemoPlayerCharacter::GetCombatSocketLocation(const FGameplayTag& InputTag)
{
	FVector Forword = GetActorForwardVector();
	Forword.Z = 0;
	return GetActorLocation() + Forword * 100;
}

void ADemoPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADemoPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//初始化ASC的ActorInfo 对于ASC位于PlayerState上
	//Server端的ActorInfo在PossessedBy初始化
	InitialAbilitySystem();
	//普攻 被动  其他技能不是默认拥有的
	UDemoSystemLibrary::GiveStartUpAbilitiesToCharacter(this,this);
	//TODO:这里做一些PS中来自Character的数据的初始化
	Cast<ADemoPlayerState>(GetPlayerState())->CharacterClass = CharacterClass;
}

//一般只调用一次，可以用来初始化
void ADemoPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//对于ASC在PlayerState上
	//Client端的ActorInfo由OnRep_PlayerState初始化
	InitialAbilitySystem();
}

int32 ADemoPlayerCharacter::GetPlayerLevel()
{
	ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(GetPlayerState());
	check(DemoPlayerState);
	return DemoPlayerState->GetPlayerLevel();
}

ETeam ADemoPlayerCharacter::GetTeam()
{
	ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(GetPlayerState());
	check(DemoPlayerState);
	return DemoPlayerState->GetTeam();
}

void ADemoPlayerCharacter::AddXP_Implementation(int32 InXP)
{
	ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(GetPlayerState());
	if(DemoPlayerState)
	{
		DemoPlayerState->AddXP(InXP);
	}
}

void ADemoPlayerCharacter::AddPlayerLevel_Implementation(int32 LevelToAdd)
{
	ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(GetPlayerState());
	if(DemoPlayerState)
	{
		DemoPlayerState->AddLevel(LevelToAdd);
	}
}

int32 ADemoPlayerCharacter::GetXP_Implementation()
{
	ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(GetPlayerState());
	if(DemoPlayerState)
	{
		return DemoPlayerState->GetXP();
	}
	return 0;
}

int32 ADemoPlayerCharacter::FindLevelFromXP_Implementation(int32 XP)
{
	ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(GetPlayerState());
	if(DemoPlayerState)
	{
		return DemoPlayerState->LevelUpInfo->FindLevelFromXP(XP);
	}
	return 1;
}

void ADemoPlayerCharacter::AddLevelPoints_Implementation(int32 InLevelPoints)
{
	ADemoPlayerState* DemoPlayerState = Cast<ADemoPlayerState>(GetPlayerState());
	if(DemoPlayerState)
	{
		DemoPlayerState->AddLevelPoints(InLevelPoints);
	}
}

void ADemoPlayerCharacter::StateCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StateCallback(CallbackTag, NewCount);
}

void ADemoPlayerCharacter::InitialAbilitySystem()
{
	if(!HasAuthority())
	{
		UE_LOG(LogTemp,Log, TEXT("ADemoPlayerCharacter::InitialAbilitySystem"));
	}
	//1 初始化ActorInfo
	ADemoPlayerState* DemoPlayerState = GetPlayerState<ADemoPlayerState>();
	check(DemoPlayerState)
	DemoPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DemoPlayerState, this);

	//2 获取当前阵营
	Team = DemoPlayerState->Team;

	//3 为PlayerCharacter中的ASC引用和AttributeSet指定PS中的对象
	AbilitySystemComponent = DemoPlayerState->GetAbilitySystemComponent();
	AttributeSet = DemoPlayerState->GetAttributeSet();

	//4 初始化Attributes OnServer(因为里面要获取GM)
	InitialDefaultAttributes();

	
	//5 初始化HUD
	//Server和Client都会调用这个函数InitialAbilitySystem初始化ASC和AS，此时WidgetController需要的PC，PS，ASC，AS都确保完成了初始化/
	//调用InitialOverlay最好
	if(ADemoPlayerController* DemoPlayerController = Cast<ADemoPlayerController>(GetController()))
	{
		if(ADemoHUD* DemoHUD = Cast<ADemoHUD>(DemoPlayerController->GetHUD()))
		{
			DemoHUD->InitOverlay(DemoPlayerController, DemoPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}


	//6 绑定事件到GameplayTag增删
	RegisterStateEvent();
	//7 Give用于实现State状态的GA
	UDemoSystemLibrary::GiveStateAbilities(this,this);
	//8 GiveAndActive与经验等有关的被动Listen触发GA
	UDemoSystemLibrary::GivePassiveAbilitiesAndActive(this,this);
}
