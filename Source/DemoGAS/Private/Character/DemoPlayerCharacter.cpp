// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGAS/Public/Character/DemoPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/DemoPlayerController.h"
#include "Player/DemoPlayerState.h"
#include "UI/HUD/DemoHUD.h"

ADemoPlayerCharacter::ADemoPlayerCharacter()
{
	//摄像机和弹簧臂基本设置
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.f,0.f,0.f));
	SpringArm->TargetArmLength = 750.f;
	//禁止摄像机继承Character旋转
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraLag = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm,USpringArmComponent::SocketName);
	
	
	//设置俯视角游戏的基本设置
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;//移动会被约束到平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;//开始时与平面对齐

	//避免摄像机视角影响角色视角
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ADemoPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADemoPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//初始化ASC的ActorInfo 对于ASC位于PlayerState上
	//Server端的ActorInfo在PossessdBy初始化
	InitialASCActorInfo();
}

//一般只调用一次，可以用来初始化
void ADemoPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//对于ASC在PlayerState上
	//Client端的ActorInfo由OnRep_PlayerState初始化
	InitialASCActorInfo();
}

void ADemoPlayerCharacter::InitialASCActorInfo()
{
	ADemoPlayerState* DemoPlayerState = GetPlayerState<ADemoPlayerState>();
	check(DemoPlayerState)
	DemoPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DemoPlayerState,this);

	
	//为PlayerCharacter中的ASC引用和AttributeSet指定PS中的对象
	AbilitySystemComponent = DemoPlayerState->GetAbilitySystemComponent();
	AttributeSet = DemoPlayerState->GetAttributeSet();

	//Server和Client都会调用这个初始化ASC和AS，此时WidgetController需要的PC，PS，ASC，AS都确保完成了初始化
	//调用InitialOverlay最好
	if(ADemoPlayerController* DemoPlayerController = Cast<ADemoPlayerController>(GetController()))
	{
		ADemoHUD* DemoHUD = Cast<ADemoHUD>(DemoPlayerController->GetHUD());
		if(DemoHUD)
		{
			DemoHUD->InitOverlay(DemoPlayerController,DemoPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
}
