// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "DemoAbilitySystemGlobals.generated.h"

/**
 * 创建这个类的子类来自定义一些类 （FGameplayEffectContext）
 */
UCLASS()
class DEMOGAS_API UDemoAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	//TODO: 单人运行正常，多人会崩溃？？？？？？？
public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
