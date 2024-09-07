// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DemoEffectActor.generated.h"

class USphereComponent;

/**
 * 用于交互后施加效果的基类
 */
UCLASS()
class DEMOGAS_API ADemoEffectActor : public AActor
{
	GENERATED_BODY()

public:
	ADemoEffectActor();

	//关于SPARSE委托和普通委托的区别
	//https://zhuanlan.zhihu.com/p/561175379
	//https://benui.ca/unreal/delegates-advanced/#sparse-delegates recommend!!
	//大概就是，SPARSE自身只有bIsBound一个bool，在不被绑定的时候只占1字节
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> InteractSphere;
};
