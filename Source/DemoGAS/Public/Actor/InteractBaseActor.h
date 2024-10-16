// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoGAS/DemoGAS.h"
#include "GameFramework/Actor.h"
#include "InteractBaseActor.generated.h"

/**
 * 作为所有普攻，指向性，指向性的基类
 * 提供：阵营，指向目标等功能
 *
 * 需要使用延时生成设定其中的参数
 */

class ADemoCharacterBase;

UCLASS()
class DEMOGAS_API AInteractBaseActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractBaseActor();

	void SetTeam(ETeam InTeam);
	FORCEINLINE ETeam GetTeam() {return Team;}
	
	void SetTargetActor(const ADemoCharacterBase* InTargetActor);
	FORCEINLINE const ADemoCharacterBase* GetTargetActor() {return TargetActor;}

protected:
	virtual void BeginPlay() override;
	
	//所属阵营
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "InteracteBase")
	ETeam Team = ETeam::ETeam_Blue;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "InteracteBaset")
	const ADemoCharacterBase* TargetActor = nullptr;
};
