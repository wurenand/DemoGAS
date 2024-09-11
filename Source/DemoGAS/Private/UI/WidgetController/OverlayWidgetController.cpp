// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "GameplayAbilities/DemoAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//不要Super （父类这个函数应该是空）
	UDemoAttributeSet* DemoAttributeSet = CastChecked<UDemoAttributeSet>(AttributeSet);
	OnHealthChangedSignature.Broadcast(DemoAttributeSet->GetHealth());
	OnMaxHealthChangedSignature.Broadcast(DemoAttributeSet->GetMaxHealth());
	OnManaChangedSignature.Broadcast(DemoAttributeSet->GetMana());
	OnMaxManaChangedSignature.Broadcast(DemoAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallBackToDependencies()
{
	UDemoAttributeSet* DemoAttributeSet = CastChecked<UDemoAttributeSet>(AttributeSet);
	//ASC提供的Attribute值改变的Delegate
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetHealthAttribute())
	                      .AddUObject(this, &UOverlayWidgetController::OnHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetMaxHealthAttribute())
	                      .AddUObject(this, &UOverlayWidgetController::OnMaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetManaAttribute())
	                      .AddUObject(this, &UOverlayWidgetController::OnManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetMaxManaAttribute())
	                      .AddUObject(this, &UOverlayWidgetController::OnMaxManaChanged);

	Cast<UDemoAbilitySystemComponent>(AbilitySystemComponent)->OnAppliedGEToSelfAssetTagsDelegate
	      .AddLambda([this](const FGameplayTagContainer& AssetTags)-> void
		      {
		        for(const FGameplayTag& Tag : AssetTags)
		        {
		        	//通过FName获取FGameplayTag(如果有的话)
		        	FGameplayTag MessageTagToCheck = FGameplayTag::RequestGameplayTag(FName("Message"));
		        	//判断是否包含MessageTag
		        	if(Tag.MatchesTag(MessageTagToCheck))
		        	{
		        		const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageDataTable, Tag); //需要捕获this使用
						OnReceiveMessageRowSignature.Broadcast(*Row);
		        	}
		        }
		      });
}

void UOverlayWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChangedSignature.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChangedSignature.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChangedSignature.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChangedSignature.Broadcast(Data.NewValue);
}
