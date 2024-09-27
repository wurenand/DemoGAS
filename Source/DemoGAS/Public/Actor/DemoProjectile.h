#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "InteractBaseActor.h"
#include "GameFramework/Actor.h"
#include "DemoProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class DEMOGAS_API ADemoProjectile : public AInteractBaseActor
{
	GENERATED_BODY()

public:
	ADemoProjectile();

	virtual void Destroyed() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	//轻量级的拿到这个Effect，并且生成时公开
	UPROPERTY(BlueprintReadWrite,meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	//用于Client避免Server的Destory复制到Client在Client的Collision发生之前，导致Client不产生效果就被销毁（感觉是只碰撞一次的时候使用）
	bool bHit = false;

	UPROPERTY(EditAnywhere,Category = "LifeSpan")
	float LifeSpan = 15.f;
protected:
	virtual void BeginPlay() override;

	//具体效果可以子类自定义
	UFUNCTION()
	void OnSphereOverlappedBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> Sphere;
	
	UPROPERTY(EditAnywhere, Category = "Impact")
	TObjectPtr<UNiagaraSystem> ImpactNiagara;

	UPROPERTY(EditAnywhere, Category = "Impact")
	TObjectPtr<USoundBase> ImpactSound;
};
