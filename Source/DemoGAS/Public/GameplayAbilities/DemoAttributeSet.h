﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "DemoAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 保存在PostEffectExecute中能得到的信息
 * Source:这个Effect的引起者
 * Target:这个Effect的目标（这个AttributeSet的所有者）
 */
USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()
	
	FEffectProperties(){}
	
	FGameplayEffectContextHandle GEContentHandle;
	UPROPERTY()
	
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvaterActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvaterActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class DEMOGAS_API UDemoAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	//在运行时使用 ~showdebug abilitysystem可以方便调试
	UDemoAttributeSet();

	/**
	 * 
	* 在CurrentValue（BaseValue + Additive，所以BaseValue被修改就相当于CurrentValue被修改）被修改之前调用
	传入的是float& 可以修改NewValue (！！注意：修改并没有永久修改modifier，只是修改了modifier返回的值，
	这意味着任何修改器GameplayEffectExecutionCalculations和ModifierMagnitudeCalculations对CurrentValue的重计算都要重新clamping)
	PreAttributeBaseChange()是BaseValue被修改前调用
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//更好的Clamp Attribute值的函数  只有BaseValue被修改且通过GE修改时才会调用
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**明确了新增AttributeData的流程：
	1.生命FGameplayAttributeData变量，标记为ReplicatedUsing
	2.利用ATTRIBUTE_ACCESSORS宏，创建访问函数
	3.在GetLifetimeReplicatedProps中注册复制变量
	4.创建相应的OnRep函数，并在函数中调用宏GAMEPLAYATTRIBUTE_REPNOTIFY来通知ASC完成了属性复制
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDemoAttributeSet,Health)
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UDemoAttributeSet,MaxHealth)
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Mana,Category = "Vital Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UDemoAttributeSet,Mana)
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Mana,Category = "Vital Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UDemoAttributeSet,MaxMana)
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue) const;
	
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& OutProps);
	
};
