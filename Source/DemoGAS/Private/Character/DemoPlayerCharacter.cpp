// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGAS/Public/Character/DemoPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
