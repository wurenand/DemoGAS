// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoGAS/Public/Character/DemoEnemyCharacter.h"

#include "Components/WidgetComponent.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "GameplayAbilities/DemoAttributeSet.h"
#include "GameplayAbilities/Library/DemoSystemLibrary.h"
#include "UI/Widget/DemoUserWidget.h"


ADemoEnemyCharacter::ADemoEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//构造ASC和AttributeSet
	AbilitySystemComponent = CreateDefaultSubobject<UDemoAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true); //TODO(为什么只给ASC设置复制?)
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UDemoAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

int32 ADemoEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

ETeam ADemoEnemyCharacter::GetTeam()
{
	return Team;
}


void ADemoEnemyCharacter::StateCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StateCallback(CallbackTag, NewCount);
}

void ADemoEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//初始化ASC的ActorInfo
	//对于AI控制的角色，直接在BeginPlay中初始化就行
	InitialAbilitySystem();
	
	//~Begin
	//设定WidgetController为Self
	if (UDemoUserWidget* DemoUserWidget = Cast<UDemoUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		DemoUserWidget->SetWidgetController(this);
	}
	//绑定属性变化委托，并传递给 WidgetController
	if (const UDemoAttributeSet* DemoAS = Cast<UDemoAttributeSet>(GetAttributeSet()))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)-> void
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)-> void
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

		//初始化一遍
		OnHealthChanged.Broadcast(DemoAS->GetHealth());
		OnHealthChanged.Broadcast(DemoAS->GetMaxHealth());
	}
	//~End
}

void ADemoEnemyCharacter::InitialAbilitySystem()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	if (HasAuthority())
	{
		InitialDefaultAttributes();
		RegisterStateEvent();
		UDemoSystemLibrary::GiveStateAbilities(this, this);
		//普攻 被动
		UDemoSystemLibrary::GiveStartUpAbilitiesToCharacter(this,this);
	}
}
