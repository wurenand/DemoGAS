// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "DemoAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAppliedGEToSelfAssetTagsDelegate,const FGameplayTagContainer& GameplayTags);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEMOGAS_API UDemoAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//在完成InitialASCActor后，自动调用。 用于绑定函数到委托
	void AfterInitialASCActorInfo();

	FOnAppliedGEToSelfAssetTagsDelegate OnAppliedGEToSelfAssetTagsDelegate;

	//只在Server调用
	void AddAbilitiesToCharacter(const TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAdd);

	//提供给外部的Input的交互接口
	void AbilityInputTagTriggered(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
protected:
	//广播消息给UI
	void OnMyGameplayEffectAppliedToSelf(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec,
	                                   FActiveGameplayEffectHandle ActiveGEHandle);

	//重写了这个函数，用于在Initial之后bind Delegate
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

private:
	//确保bind Delegate只做一次
	bool bAfterInitASCActorInfo = false;
};
