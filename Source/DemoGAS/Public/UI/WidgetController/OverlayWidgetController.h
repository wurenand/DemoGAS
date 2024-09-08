// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoWidgetController.h"
#include "OverlayWidgetController.generated.h"

//声明委托用于广播Value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature,float ,NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature,float ,NewMaxHealth);

/**
 * 
 */
UCLASS()
class DEMOGAS_API UOverlayWidgetController : public UDemoWidgetController
{
	GENERATED_BODY()

public:
	//子类重写 用于初始化的广播，应当在Widget设置其WidgetController之后就被调用
	virtual void BoardcastInitialValues() override;
	//子类重写 用于绑定函数到AttributeChanged
	virtual void BindCallBackToDependencies() override;

	UPROPERTY(BlueprintAssignable , Category = "GAS|Attributes")
	FOnHealthChangedSignature OnHealthChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChangedSignature;

protected:
	//~ Attribute值变化的用于bind的函数
	void OnHealthChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data) const;
};
