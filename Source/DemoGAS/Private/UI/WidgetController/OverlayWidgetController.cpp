// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "GameplayAbilities/DemoAttributeSet.h"

void UOverlayWidgetController::BoardcastInitialValues()
{
	//不要Super （父类这个函数应该是空）
	UDemoAttributeSet* DemoAttributeSet = CastChecked<UDemoAttributeSet>(AttributeSet);
	OnHealthChangedSignature.Broadcast(DemoAttributeSet->GetHealth());
	OnMaxHealthChangedSignature.Broadcast(DemoAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallBackToDependencies()
{
	UDemoAttributeSet* DemoAttributeSet = CastChecked<UDemoAttributeSet>(AttributeSet);
	//ASC提供的Attribute值改变的Delegate
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetHealthAttribute())
	                      .AddUObject(this, &UOverlayWidgetController::OnHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetMaxHealthAttribute())
	                      .AddUObject(this, &UOverlayWidgetController::OnMaxHealthChanged);
}

void UOverlayWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChangedSignature.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChangedSignature.Broadcast(Data.NewValue);
}
