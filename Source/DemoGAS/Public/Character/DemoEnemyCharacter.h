// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoCharacterBase.h"
#include "Interface/InteractInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "DemoEnemyCharacter.generated.h"

class UWidgetComponent;



UCLASS()
class DEMOGAS_API ADemoEnemyCharacter : public ADemoCharacterBase
{
	GENERATED_BODY()

public:
	ADemoEnemyCharacter();
	

	//~Begin CombatInterface
	virtual int32 GetPlayerLevel() override;
	//~End

	//~Begin HealthBar组件要绑定的委托 (来自WidgetController)
	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeValueChangedSignature OnMaxHealthChanged;
	//~End

	virtual void StateCallback(const FGameplayTag CallbackTag, int32 NewCount) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void InitialAbilitySystem() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "CharacterDefault")
	int32 Level = 1; //AIController只存在于Server，所以不需要复制

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "UI")
	TObjectPtr<UWidgetComponent> HealthBar;
};
