// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DemoEffectActor.generated.h"

class UGameplayEffect;

/**
 * 用于交互后施加效果的基类
 */
UCLASS()
class DEMOGAS_API ADemoEffectActor : public AActor
{
	GENERATED_BODY()

public:
	ADemoEffectActor();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyGameplayEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "GAS|Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category = "GAS|Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
private:
	//视觉效果Mesh和碰撞体Component由蓝图实现。
	//C++只负责处理碰撞后的逻辑
};
