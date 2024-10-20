// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FDemoAbilityInfo;
class UAbilityInfo;
class UDemoUserWidget;

/**
 * DataTable行结构，作为ASC与WidgetController之间沟通的Message格式
 */
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message = FText();
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UDemoUserWidget> MessageWidget;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image = nullptr;
};

//声明委托用于广播Value
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeValueChangedSignature,float ,NewValue);
//受到GEToSelf后，会受到来自ASC的GEMessage，再由它Broadcast给Widget
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveMessageRowSignature,FUIWidgetRow , WidgetRow);
//广播GA的信息
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGivenAbilitySignature,const FDemoAbilityInfo& ,Info);

/**
 * 
 */
UCLASS()
class DEMOGAS_API UOverlayWidgetController : public UDemoWidgetController
{
	GENERATED_BODY()

public:
	//子类重写 用于初始化的广播，应当在Widget设置其WidgetController之后就被调用
	virtual void BroadcastInitialValues() override;
	//子类重写 用于绑定函数到AttributeChanged
	virtual void BindCallBackToDependencies() override;

	//~Begin Value 委托
	UPROPERTY(BlueprintAssignable , Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnHealthChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnMaxHealthChangedSignature;
	UPROPERTY(BlueprintAssignable , Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnManaChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnMaxManaChangedSignature;

	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnAttackDistanceChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnStrengthChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnResilienceChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnIntelligenceChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnVigorChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnArmorChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnSpellArmorChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnCriticalChanceChangedSignature;
	UPROPERTY(BlueprintAssignable ,Category = "GAS|Attributes")
	FOnAttributeValueChangedSignature OnCriticalBoncePercentChangedSignature;
	//~End
	
	UPROPERTY(BlueprintAssignable,Category = "WidgetMessage")
	FOnReceiveMessageRowSignature OnReceiveMessageRowSignature;

	UPROPERTY(BlueprintAssignable,Category = "AbilityInfo")
	FOnGivenAbilitySignature OnGivenAbilitySignature;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "WidgetMessageData")
	TObjectPtr<UDataTable> MessageDataTable;//传递GETag消息的表

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AbilityInfos")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	//T为传入的TableRow的类型
	template <typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& Tag);
};

//采用包含模式，模板函数的声明和定义都在h文件中
template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
