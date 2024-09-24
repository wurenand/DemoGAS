// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCursorTargetDataSignature,const FGameplayAbilityTargetDataHandle&, Data);

//TODO:这里的整套发送TargetData的流程和预测需要研究
/**
 * 获取当前光标信息
 * static函数作为工厂返回一个自身的示例
 * 输出执行引脚是通过委托实现
 * 当执行到此结点的时候，会调用Active函数
 */
UCLASS()
class DEMOGAS_API UAbilityTask_TargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()

public:
	//BlueprintInternalUseOnly = "TRUE"表示只在蓝图内部使用
	/**
	 * 获取当前光标信息 
	 */
	UFUNCTION(BlueprintCallable,Category = "Ability|Tasks",meta=(HidePin="OwningAbility",DefaultToSelf="OwningAbility",BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_TargetDataUnderCursor* CreateTargetDataUnderCursor(UGameplayAbility* OwningAbility);

	//委托的成员变量名ValidData会成为输出引脚的名字，委托的参数会成为输出参数之一。当广播这个委托的时候，输出引脚会被执行
	UPROPERTY(BlueprintAssignable)
	FCursorTargetDataSignature ValidData;

private:
	//当执行到此结点的时候，会调用
	virtual void Activate() override;

	//RPC，从客户端发送当前鼠标数据到Server
	void SendCursorTargetData();

	//Server收到TargetData的Callback
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag);
};
