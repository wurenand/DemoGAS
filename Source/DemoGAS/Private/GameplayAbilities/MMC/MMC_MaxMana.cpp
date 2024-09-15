// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/MMC/MMC_MaxMana.h"

#include "GameplayAbilities/DemoAttributeSet.h"
#include "Interface/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDefinition.AttributeToCapture = UDemoAttributeSet::GetIntelligenceAttribute();
	IntelligenceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDefinition);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDefinition,Spec,EvaluateParameters,Intelligence);
	
	//获取Level
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	check(CombatInterface);
	int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 20.f + 2.5f * Intelligence + 10.f * PlayerLevel;
	
}
