// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "InteractBaseActor.h"
#include "GameFramework/Actor.h"
#include "DemoEffectActor.generated.h"

class UAbilitySystemComponent;

//应用和移除GE的策略 枚举
UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnBeginOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
};

UENUM(BlueprintType)
enum class EEffectRemovePolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove,
};

class UGameplayEffect;

/**
 * 用于交互后施加效果的基类
 * 在编辑器中指定三种EffectClass，并设定枚举值来决定应用哪种Effect，和Effect的Remove
 */

//视觉效果Mesh和碰撞体Component由蓝图实现。
//C++只负责处理碰撞后的逻辑
UCLASS()
class DEMOGAS_API ADemoEffectActor : public AInteractBaseActor
{
	GENERATED_BODY()

public:
	ADemoEffectActor();

protected:
	virtual void BeginPlay() override;

	//~蓝图中调用这两个函数，其他的交给CPP处理
	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	//~end

	UFUNCTION(BlueprintCallable)
	void ApplyGameplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Effects|Setting")
	bool bDestroyAfterGE = false; //GE被移除后是否摧毁Actor （暂时没有实现）

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS|Effects|Setting")
	float EffectLevel = 1.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Effects|Class")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Effects|Policy")
	EEffectApplicationPolicy InstantGEApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Effects|Class")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Effects|Policy")
	EEffectApplicationPolicy DurationGEApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Effects|Class")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Effects|Policy")
	EEffectApplicationPolicy InfiniteGEApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Effects|Policy")
	EEffectRemovePolicy InfiniteGERemovePolicy = EEffectRemovePolicy::RemoveOnEndOverlap;

private:
	//存储每个Actor的ASC到对应的Active的InfiniteGE的映射，用于在ActorEndOverlap之后remove这个InfiniteGE
	TMap<UAbilitySystemComponent*, FActiveGameplayEffectHandle> InfiniteOverlapActors;
};
