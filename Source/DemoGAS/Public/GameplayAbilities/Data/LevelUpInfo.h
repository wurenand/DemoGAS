#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * 与Level有关的信息
 * 包括每一级需要的总XP
 * 每次升级的奖励...
 */
UCLASS()
class DEMOGAS_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//每次升级奖励的可分配的技能点
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelUpInfo")
	int32 SpellPointAward = 1;

	//记录每级的总XP(从2级开始)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelUpInfo")
	TArray<int32> LevelUpRequirementXPS;
	
	UFUNCTION(BlueprintCallable, Category = "LevelUpInfo")
	int32 FindLevelFromXP(int32 XP);
};
