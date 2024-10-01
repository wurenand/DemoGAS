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

	UPROPERTY(EditDefaultsOnly,Category = "ClassDefaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributesEffectClass;	
};

/**
 * 英雄的信息，包括GA，数值表，GE
 */
//TODO:是不是还应该有GA呢？
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

	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
