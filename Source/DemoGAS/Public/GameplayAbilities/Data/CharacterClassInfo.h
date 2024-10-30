// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class ADemoPlayerCharacter;
class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	//TODO:这里选择不同英雄
	ECC_Ezreal UMETA(DisplayName = "伊泽瑞尔"),
	ECC_JINZHAN  UMETA(DisplayName = "近战英雄占位符"),
	ECC_TEST2  UMETA(DisplayName = "测试角色"),
};

/**
 * 每个Character独享的部分
 */
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	//角色对应的Pawn
	UPROPERTY(EditDefaultsOnly,Category="CharacterDefaults|BPClass")
	TSubclassOf<ADemoPlayerCharacter> PlayerCharacterBPClass;
	
	UPROPERTY(EditDefaultsOnly,Category = "ClassDefaults|GE|PrimaryAttributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributesEffectClass;

	UPROPERTY(EditDefaultsOnly,Category = "ClassDefaults|GA|NormalAttack")
	TSubclassOf<UGameplayAbility> NormalAttackClass;
	
	UPROPERTY(EditDefaultsOnly,Category = "ClassDefaults|GA|PassiveAbility")
	TSubclassOf<UGameplayAbility> PassiveAbilityClass;

	// Q W E R
	UPROPERTY(EditDefaultsOnly , Category = "ClassDefaults|GA|AbilitiesAttack")
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesClass;
};

/**
 * 英雄的信息，包括GA，数值表，GE
 */
UCLASS()
class DEMOGAS_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	//独立的数组数据
	UPROPERTY(EditDefaultsOnly,Category = "CharacterClassDefaults")
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInformation;
	
	//关于Health和Mana的设置 （第一次初始化的时候Apply用于把Health或者Mana回复满，后续升级的时候就Skip）
	UPROPERTY(EditDefaultsOnly,Category = "CommonClassDefaults")
	TSubclassOf<UGameplayEffect> VitalAttributesEffectClass;

	UPROPERTY(EditDefaultsOnly,Category = "StateAbilities")
	TArray<TSubclassOf<UGameplayAbility>> StateAbilities;
	//TODO: 把生命回复和蓝量回复做到这里
	//与经验等有关的PassiveAbility 默认就激活
	UPROPERTY(EditDefaultsOnly,Category = "DataPassiveAbilities")
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilities;

	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
