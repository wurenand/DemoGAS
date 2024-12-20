﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DemoGAS/DemoGAS.h"
#include "GameFramework/Character.h"
#include "GameplayAbilities/Abilities/DemoGameplayAbilityBase.h"
#include "GameplayAbilities/Data/CharacterClassInfo.h"
#include "Interface/CombatInterface.h"
#include "Interface/InteractInterface.h"
#include "DemoCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
//Abstract修饰符标识这个类不能被实例
UCLASS(Abstract)
class DEMOGAS_API ADemoCharacterBase : public ACharacter, public IAbilitySystemInterface,public IInteractInterface,public ICombatInterface //继承这个Combat接口，再有子类实现
{
	GENERATED_BODY()

public:
	ADemoCharacterBase();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//来自IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//自定义，仿制上面的函数，给AttributeSet一个Getter
	UAttributeSet* GetAttributeSet() const;

	//~Begin InteractInterface
	virtual void HighlightActor(ETeam FromTeam) override;
	virtual void UnHighlightActor() override;
	//~End

	USceneComponent* GetTargetComponent() const {return TargetComponent;};

	//~Begin CombatInterface
	virtual UAnimMontage* GetStunnedMontage_Implementation() override;
	virtual int32 GetPlayerLevel() override;
	virtual ETeam GetTeam() override;
	virtual void Die(UDemoAbilitySystemComponent* Killer) override;
	//~End

	//处理死亡的函数，Run On Server和Client
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();
	

	UPROPERTY(EditAnywhere,Category = "CharacterClassInfo")
	ECharacterClass CharacterClass = ECharacterClass::ECC_Ezreal;
	//用来调用GE来初始化Attributes 在子类的InitialAbilitySystem调用
	virtual void InitialDefaultAttributes() const;//由子类实现
	//（第一次初始化的时候Apply用于把Health或者Mana回复满，后续升级的时候就Skip）
	bool bFirstInitial = false;
	
	//~end

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClassToBeApplied,float Level) const;

protected:

	/*使用TObjectPtr的优点:在打包后会被解释为原始指针，与原始指针无异
	 *但在编辑器中，提供了
	 *1.访问追踪：跟踪指针被访问或者取消引用的频率
	 *2.Lazy Load:只在需要的时候加载资产
	 */
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	//用作追踪形技能的目标点（趋于中间）
	UPROPERTY(EditAnywhere, Category = "Component")
	TObjectPtr<USceneComponent> TargetComponent;

	//对于PlayerCharacter：只是PlayerState中构造的ASC和AttributeSet的引用，并不实际构造
	//对于EnemyCharacter：实际构造
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	//由子类去实现，用于初始化ASC的一些内容 具体可见DemoPlayerCharacter中的函数实现   (Server和Client都会调用)
	virtual void InitialAbilitySystem();

	//注册事件到指定的(state)GameplayTag添加或移除 （建议子类要在InitialAbilitySystem的最后手动调用）
	void RegisterStateEvent();
	//State Callback函数
	virtual void StateCallback(const FGameplayTag CallbackTag,int32 NewCount);

	//阵营变量，只是给Enemy用的，Player的Team从PS中获取
	UPROPERTY(EditAnywhere,ReplicatedUsing = "OnRep_Team",Category = "Team") //TODO:暂时先设置为EditAnyWhere
	ETeam Team = ETeam::ETeam_Red;


	//眩晕蒙太奇动画
	UPROPERTY(EditDefaultsOnly,Category = "Montage")
	TObjectPtr<UAnimMontage> StunnedMontage;
	
private:
	UFUNCTION()
	void OnRep_Team();

};
