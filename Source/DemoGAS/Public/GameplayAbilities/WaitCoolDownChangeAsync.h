// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCoolDownChangeAsync.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoolDownChangeDelegate, float, CoolDownRemaining);

/**
 * 异步节点 广播GA的CoolDown
 */
UCLASS()
class DEMOGAS_API UWaitCoolDownChangeAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	public:
	UPROPERTY(BlueprintAssignable)
	FOnCoolDownChangeDelegate OnCoolDownStart;

	UPROPERTY(BlueprintAssignable)
	FOnCoolDownChangeDelegate OnCoolDownEnd;

	UFUNCTION(BlueprintCallable,meta=(BlueprintInternalUseOnly = "true"))
	static UWaitCoolDownChangeAsync* WaitCoolDownChange(UAbilitySystemComponent* InASC,const FGameplayTag& InCoolDownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();//回收资源

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CoolDownTag;

	void CoolDownChanged(const FGameplayTag InCoolDownTag,int NewCount);
	//绑定到DurationGE被Apply之后的回调 用于得到这个GE的Duration时长
	void GameplayEffectAppliedToSelf(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpec,FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
