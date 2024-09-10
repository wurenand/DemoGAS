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

void ADemoEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	//依次判断情况
	if(InstantGEApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if(DurationGEApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if(InfiniteGEApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void ADemoEffectActor::OnEndOverlap(AActor* TargetActor)
{
	//依次判断情况
	if(InstantGEApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if(DurationGEApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if(InfiniteGEApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	if(InfiniteGERemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		//remove
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(!TargetASC) return;
		FActiveGameplayEffectHandle* ActiveGESpecToBeRemoved = InfiniteOverlapActors.Find(TargetASC);
		if(!ActiveGESpecToBeRemoved) return;
		//用handle移除GE  //只移除一个stack(避免由于重合导致的Stack全部被移除问题)
		TargetASC->RemoveActiveGameplayEffect(*ActiveGESpecToBeRemoved , 1);
		//从Map中移除键值对
		InfiniteOverlapActors.Remove(TargetASC);
	}
}

void ADemoEffectActor::ApplyGameplayEffectToTarget(AActor* TargetActor,
                                                   TSubclassOf<UGameplayEffect> GameplayEffectClass)
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
	checkf(GameplayEffectClass, TEXT("传入的GameplayEffectClass是空"))

	//Handle只是一个Wrapper，真正的数据存在其中的 Data 成员变量
	FGameplayEffectContextHandle GEContextHandle = TargetASC->MakeEffectContext();
	//FGameplayEffectContent具有存储与GameplayEffect相关信息的能力
	GEContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle GESpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,EffectLevel, GEContextHandle);
	FActiveGameplayEffectHandle ActiveGEHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());


	//处理Infinite类型的Remove
	//注意这里访问的是UGameplayEffect的CDO，不能修改数值！
	bool bIsInfinite = GESpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	//只有是Infinite类型，并且需要在EndOverlap的时候remove的GE。才需要把Target添加到TMap
	if(bIsInfinite && InfiniteGERemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		InfiniteOverlapActors.Add(TargetASC,ActiveGEHandle);
	}
}
