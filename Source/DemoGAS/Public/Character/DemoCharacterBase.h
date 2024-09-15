﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interface/CombatInterface.h"
#include "DemoCharacterBase.generated.h"

class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
//Abstract修饰符标识这个类不能被实例
UCLASS(Abstract)
class DEMOGAS_API ADemoCharacterBase : public ACharacter, public IAbilitySystemInterface,public ICombatInterface //继承这个Combat接口，再有子类实现
{
	GENERATED_BODY()

public:
	ADemoCharacterBase();

	//来自IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//自定义，仿制上面的函数，给AttributeSet一个Getter
	UAttributeSet* GetAttributeSet() const;

protected:

	/*使用TObjectPtr的优点:在打包后会被解释为原始指针，与原始指针无异
	 *但在编辑器中，提供了
	 *1.访问追踪：跟踪指针被访问或者取消引用的频率
	 *2.Lazy Load:只在需要的时候加载资产
	 */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	//对于PlayerCharacter：只是PlayerState中构造的ASC和AttributeSet的引用，并不实际构造
	//对于EnemyCharacter：实际构造
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	//由子类去实现，用于初始化ASC的一些内容 具体可见DemoPlayerCharacter中的函数实现   (Server和Client都会调用)
	virtual void InitialAbilitySystem();

	//~begin 属性初始化与设置
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AttributeEffect")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesEffectClass;
	//Secondary属性使用InfiniteEffect来不断追踪Primary属性的变化而变化
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="AttributeEffect")
	TSubclassOf<UGameplayEffect> SecondaryAttributesUpdateEffectClass;
	
	//用来调用GE来初始化Attributes 和 InfiniteGE来不断更新Secondary  在子类的InitialAbilitySystem调用
	void InitialDefaultAttributes() const;//由子类实现
	
	//~end

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClassToBeApplied,float Level) const;
	

};
