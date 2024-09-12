// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGAS/Public/Character/DemoEnemyCharacter.h"

#include "DemoGAS/DemoGAS.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "GameplayAbilities/DemoAttributeSet.h"


ADemoEnemyCharacter::ADemoEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	//设定后PC中的CursorTrace才能检测到
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	//构造ASC和AttributeSet
	AbilitySystemComponent = CreateDefaultSubobject<UDemoAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true); //TODO(为什么只给ASC设置复制?)
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UDemoAttributeSet>(TEXT("AttributeSet"));
}

void ADemoEnemyCharacter::HighlightActor()
{
	//理由后期处理材质，实现描边（一定要在项目设置里面打开 自定义深度的 启动模板）
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponMesh->SetRenderCustomDepth(true);
	WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ADemoEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);
}


void ADemoEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//初始化ASC的ActorInfo
	//对于AI控制的角色，直接在BeginPlay中初始化就行
	InitialASCActorInfo();
}

void ADemoEnemyCharacter::InitialASCActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	//初始化Attributes，可以只在Server调用，但是在这个函数中Server和Client都会调用，在Client调用可以避免Client等待同步的过程
	InitialPrimaryAttributes();
}

