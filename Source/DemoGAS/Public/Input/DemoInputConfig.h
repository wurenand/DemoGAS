// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DemoInputConfig.generated.h"

class UInputAction;

/**
 * 自定义的输入操作的结构
 */
USTRUCT(BlueprintType)
struct FDemoInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
	
};

/**
 * 仿制Lyra
 */
UCLASS()
class DEMOGAS_API UDemoInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * 返回根据Tag的对应InputAction
	 * @param Tag 需要寻找的Tag
	 * @param bLogNotFound true则会在没找到的时候记录UE_LOG
	 * @return 未找到就会返回nullptr
	 */
	UFUNCTION(BlueprintCallable)
	const UInputAction* FindInputActionFromTag(const FGameplayTag& Tag,bool bLogNotFound);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FDemoInputAction> AbilityInputActions;
	
};
