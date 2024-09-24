﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DemoProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class DEMOGAS_API ADemoProjectile : public AActor
{
	GENERATED_BODY()

public:
	ADemoProjectile();

	UPROPERTY(VisibleAnywhere,Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlappedBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,
		UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere,Category = "Collision")
	TObjectPtr<USphereComponent> Sphere;
};