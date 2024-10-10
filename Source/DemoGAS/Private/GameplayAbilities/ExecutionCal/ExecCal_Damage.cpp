﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/ExecutionCal/ExecCal_Damage.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "GameplayAbilities/DemoAttributeSet.h"

/**
 * 存储需要捕获的Attributes
 */
struct DemoDamageStatics
{
	//利用Marco来实现属性的捕获1 创建捕获Definiton
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	DemoDamageStatics()
	{
		//利用Marco来实现属性的捕获2 设定捕获属性，捕获来源，是否快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDemoAttributeSet, Armor, Target, false);
	}
};

//创建一个静态函数来创建与获取静态的DemoDamageStatics，确保每次使用的都是一个
static const DemoDamageStatics& DamageStatics()
{
	//创建一个静态局部变量
	static DemoDamageStatics DStatics;
	return DStatics;
}

UExecCal_Damage::UExecCal_Damage()
{
	//利用Marco来实现属性的捕获3 添加到捕获列表
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UExecCal_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//TODO:标准流程
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;


	//执行计算
	//拿到SetByCaller的值
	float Damage = Spec.GetSetByCallerMagnitude(UGameplayTagsManager::Get().RequestGameplayTag(FName("DamageCaller")));

	float TargetArmor = 0.f;
	//利用Marco来实现属性的捕获4 实际捕获属性
	TargetArmor = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	//TODO:为什么要Clamp一次？
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	
	Damage = (100 / (100 + TargetArmor)) * Damage;

	//最终输出修改
	FGameplayModifierEvaluatedData EvaluatedData(UDemoAttributeSet::GetInComingDamageAttribute(),
	                                             EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}