// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoCharacterBase.h"
#include "Interface/PlayerInterface.h"
#include "DemoPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class DEMOGAS_API ADemoPlayerCharacter : public ADemoCharacterBase,public IPlayerInterface
{
	GENERATED_BODY()

public:
	ADemoPlayerCharacter();

	//根据不同Hero的不同技能（按照InputTag区分）来返回不同的ProjectileGA的生成位置
	virtual FVector GetCombatSocketLocation(const FGameplayTag& InputTag) override;
	
protected:
	virtual void BeginPlay() override;

	//只会在Server调用
	virtual void PossessedBy(AController* NewController) override;
	//默认只在Client调用 并且一般只调用一次（变化后才会再次调用）
	virtual void OnRep_PlayerState() override;

	//~Begin CombatInterface
	virtual int32 GetPlayerLevel() override;
	virtual ETeam GetTeam() override;
	//~End

	//~begin PlayerInterface
	virtual void AddXP_Implementation(int32 InXP) override;
	virtual void AddPlayerLevel_Implementation(int32 LevelToAdd) override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 FindLevelFromXP_Implementation(int32 XP) override;
	virtual void AddLevelPoints_Implementation(int32 InLevelPoints) override;
	//~end
	
	virtual void StateCallback(const FGameplayTag CallbackTag, int32 NewCount) override;
	
private:
	UPROPERTY(EditAnywhere,Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(EditAnywhere,Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	
	virtual void InitialAbilitySystem() override;
};
