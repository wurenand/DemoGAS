// Fill out your copyright notice in the Description page of Project Settings.

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
 * 
 */
UCLASS()
class DEMOGAS_API UDemoAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	//在运行时使用 ~showdebug abilitysystem可以方便调试
	UDemoAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*明确了新增AttributeData的流程：
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
};
