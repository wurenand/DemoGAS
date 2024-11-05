// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DemoDataAssetLibrary.generated.h"

class UCharacterClassInfo;
/**
 * 提供函数方便蓝图能够访问到DataAsset
 */
UCLASS()
class DEMOGAS_API UDemoDataAssetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//CCI:获取所有E to Icon
	UFUNCTION(BlueprintCallable, Category = "DataAssets|CharacterClassInfo")
	static UCharacterClassInfo* GetCharacterClassInfo(UCharacterClassInfo* DataAsset);
};
