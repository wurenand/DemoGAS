// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	//TODO:这里选择不同英雄
	ECC_Ezreal UMETA(DisplayName = "伊泽瑞尔"),
	ECC_JINZHAN  UMETA(DisplayName = "伊泽瑞尔2"),
	ECC_TEST2  UMETA(DisplayName = "伊泽瑞尔3"),
};

/**
 * 每个Character独享的部分
 */
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Category = "ClassDefaults|GE|PrimaryAttributes")
	TSubclassOf<UGameplayEffect> PrimaryAttributesEffectClass;

	UPROPERTY(EditDefaultsOnly,Category = "ClassDefaults|GA|NormalAttack")
	TSubclassOf<UGameplayAbility> NormalAttackClass;

	//最多5个吧 (QWER和被动)
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
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInfomation;
	
	//每种英雄共享的部分
	
	UPROPERTY(EditDefaultsOnly,Category = "CommonClassDefaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributesEffectClass;
	UPROPERTY(EditDefaultsOnly,Category = "CommonClassDefaults")
	TSubclassOf<UGameplayEffect> VitalAttributesEffectClass;

	UPROPERTY(EditDefaultsOnly,Category = "StateAbilities")
	TArray<TSubclassOf<UGameplayAbility>> StateAbilities;
	//与经验等有关的PassiveAbility 默认就激活
	UPROPERTY(EditDefaultsOnly,Category = "DataPassiveAbilities")
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilities;

	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
