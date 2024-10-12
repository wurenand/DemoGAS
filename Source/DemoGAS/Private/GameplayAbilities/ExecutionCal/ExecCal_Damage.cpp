// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/ExecutionCal/ExecCal_Damage.h"

#include "AbilitySystemComponent.h"
#include "DemoAbilitySystemTypes.h"
#include "GameplayTagsManager.h"
#include "GameplayAbilities/DemoAttributeSet.h"
#include "GameplayAbilities/Abilities/DamageGameplayAbility.h"
#include "GameplayAbilities/Library/DemoSystemLibrary.h"

/**
 * 存储需要捕获的Attributes
 */
struct DemoDamageStatics
{
	//利用Marco来实现属性的捕获1 创建捕获Definiton
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(SpellArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalChance)

	DemoDamageStatics()
	{
		//利用Marco来实现属性的捕获2 设定捕获属性，捕获来源，是否快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDemoAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDemoAttributeSet, SpellArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDemoAttributeSet, CriticalChance, Source, false);
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
	RelevantAttributesToCapture.Add(DamageStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().SpellArmorDef);
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

	//拿到DamageType

	if (const UDamageGameplayAbility* DamageGA = Cast<UDamageGameplayAbility>(Spec.GetContext().GetAbility()))
	{
		EDamageType DamageType = DamageGA->GetDamageType();
		switch (DamageType)
		{
		case EDamageType::Damage_Physics:
			//利用Marco来实现属性的捕获4 实际捕获属性
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
				DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
			break;
		case EDamageType::Damage_Magic:
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
				DamageStatics().SpellArmorDef, EvaluateParameters, TargetArmor);
			break;
		case EDamageType::Damage_True:
			TargetArmor = 0.f;
			break;
		}
	}

	//TODO: 是否应该改到造成伤害的那一边呢？
	//捕获暴击率并计算是否暴击
	float CriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalChanceDef, EvaluateParameters, CriticalChance);
	CriticalChance = FMath::Max<float>(0.f, CriticalChance);
	const bool bIsCriticalHit = FMath::RandRange(1, 100) < CriticalChance * 100;

	//拿到Context 传递信息演示
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
	UDemoSystemLibrary::SetIsCriticalHit(ContextHandle, bIsCriticalHit);


	//TODO:换成Attribute的暴击伤害率
	//计算暴击伤害
	Damage = bIsCriticalHit ? Damage * 2 : Damage;
	//计算削去护甲的伤害
	Damage = (100 / (100 + TargetArmor)) * Damage;

	//最终输出修改
	FGameplayModifierEvaluatedData EvaluatedData(UDemoAttributeSet::GetInComingDamageAttribute(),
	                                             EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}