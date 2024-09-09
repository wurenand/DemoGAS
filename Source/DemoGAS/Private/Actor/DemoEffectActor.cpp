// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DemoEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

ADemoEffectActor::ADemoEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	//场景组件作为Root，给蓝图作为Root
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
}

void ADemoEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void ADemoEffectActor::ApplyGameplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	
	//使用ASC函数库来辅助完成获取ASC
	/**
 * 等效于传统IAbilitySystemInterface
* 	if(IAbilitySystemInterface* Interface = Cast<IAbilitySystemInterface>(Target))
{
	Interface->GetAbilitySystemComponent();
}
 */
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	//健壮性检查
	if(!TargetASC) return;
	checkf(GameplayEffectClass,TEXT("传入的GameplayEffectClass是空"))
	
	//Handle只是一个Wrapper，真正的数据存在其中的 Data 成员变量
	FGameplayEffectContextHandle GameplayEffectContextHandle = TargetASC->MakeEffectContext();
	//FGameplayEffectContent具有存储与GameplayEffect相关信息的能力
	GameplayEffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,1.0f,GameplayEffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
	
}



