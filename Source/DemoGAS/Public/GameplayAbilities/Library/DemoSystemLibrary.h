// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DemoSystemLibrary.generated.h"

class ADemoCharacterBase;
/**
 * 蓝图函数库，提供一些函数
 */
UCLASS()
class DEMOGAS_API UDemoSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//给传入的CharacterBase，根据CharacterClass施加相应的GE，GA，甚至包括模型替换等....
	UFUNCTION(BlueprintCallable, Category = "DemoSystemLibrary|CharacterClassDefaults")
	static void InitialDefaultAttributes(const UObject* WorldContentObject, ECharacterClass CharacterClass, float Level,
	                                     ADemoCharacterBase* CharacterBase);

	//根据CharacterClassInfo给予共享的GA（实现State的GA）
	static void GiveStateAbilities(const UObject* WorldContentObject,ADemoCharacterBase* CharacterBase);
};
