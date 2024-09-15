// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/MMC/MMC_MaxHealth.h"

#include "GameplayAbilities/DemoAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDefinition.AttributeToCapture = UDemoAttributeSet::GetVigorAttribute(); //标记要捕获的是哪个Attribute(决定类型)
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; //指定获取AttributeData的值的来源(根据类型决定数值)
	/**
	 * 关于bSnapshot
	 * true:使用在GE被创建的时候的值，保存一个快照
	 * false:使用在GE被Apply的时候的值
	 */
	VigorDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDefinition); //添加进相关AttributeToCapture中
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//TODO:什么意思
	//拿到Source和Target的Tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	//获取活力值
	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDefinition,Spec,EvaluateParameters,Vigor);
	Vigor = FMath::Max<float>(Vigor,0.f);
	//获取Level
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	check(CombatInterface);
	int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}

