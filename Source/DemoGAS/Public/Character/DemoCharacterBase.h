// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DemoCharacterBase.generated.h"

//Abstract修饰符标识这个类不能被实例
UCLASS(Abstract)
class DEMOGAS_API ADemoCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADemoCharacterBase();

protected:
	virtual void BeginPlay() override;

	/*使用TObjectPtr的优点:在打包后会被解释为原始指针，与原始指针无异
	 *但在编辑器中，提供了
	 *1.访问追踪：跟踪指针被访问或者取消引用的频率
	 *2.Lazy Load:只在需要的时候加载资产
	 */
	UPROPERTY(EditAnywhere,Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	
};
