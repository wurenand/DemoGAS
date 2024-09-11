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
	                      .AddLambda([this](const FOnAttributeChangeData& Data)->void
	                      {
		                      OnHealthChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetMaxHealthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)->void
	                      {
		                     OnMaxHealthChangedSignature.Broadcast(Data.NewValue); 
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetManaAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)->void
	                      {
		                     OnManaChangedSignature.Broadcast(Data.NewValue); 
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetMaxManaAttribute())
						  .AddLambda([this](const FOnAttributeChangeData& Data)->void
						  {
						  	OnMaxManaChangedSignature.Broadcast(Data.NewValue); 
						  });

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
