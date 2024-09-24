// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Tasks/AbilityTask_TargetDataUnderCursor.h"

#include "AbilitySystemComponent.h"

UAbilityTask_TargetDataUnderCursor* UAbilityTask_TargetDataUnderCursor::CreateTargetDataUnderCursor(
	UGameplayAbility* OwningAbility)
{
	UAbilityTask_TargetDataUnderCursor* MyObj = NewAbilityTask<UAbilityTask_TargetDataUnderCursor>(OwningAbility);
	return MyObj;
}

/**
 * 传递过程：Client调用Active，通知Server调用Active，Server接收到有延迟
 *         Client获取TargetData并传递给Server，Server再接收到也有延迟。
 *         为保证Server处理逻辑的时候，一定收到了TargetData
 *         将逻辑抽象到ReplicatedTargetDataDelegate的回调函数中，
 *         当Server允许Active的时候，绑定这个Callback到Delegate，这样就能在收到TargetData的时候通过broadcast调用（Active先到达）
 *         若是TargetData先到达，再收到Active的时候绑定Delegate也不会触发了，所以要手动调用一次CallReplicatedTargetDataDelegatesIfSet
 */
void UAbilityTask_TargetDataUnderCursor::Activate()
{
	//获取激活这个能力的Actor的本地控制信息来区分是Server端的Active还是Client端的Active
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	//本地操控的需要发送数据到Server
	if(bIsLocallyControlled)
	{
		SendCursorTargetData();
	}
	else
	{
		//这个能力激活的Server端 （Client激活后会发送Server激活的请求，这段代码是Server激活后运行）
		//绑定Callback到Server接收到TargetData的Delegate
		FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		FPredictionKey PredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent->AbilityTargetDataSetDelegate(AbilitySpecHandle,PredictionKey )
		                      .AddUObject(this, &UAbilityTask_TargetDataUnderCursor::OnTargetDataReplicatedCallback);
		//手动调用
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle,PredictionKey);
		//Active先到达Server的情况
		if(!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAbilityTask_TargetDataUnderCursor::SendCursorTargetData()
{
	//表示允许预测？？
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	//利用GameplayAbilityTargetData将Client本地的鼠标信息发送给Server
	FGameplayAbilityTargetData_SingleTargetHit* TargetHit = new FGameplayAbilityTargetData_SingleTargetHit();
	//获取结果
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult Result;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, Result);
	TargetHit->HitResult = Result;

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(TargetHit);
	//发送
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	if(ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UAbilityTask_TargetDataUnderCursor::OnTargetDataReplicatedCallback(
	const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	//通知ASC已经收到了TargetData，可以清理缓存了
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
