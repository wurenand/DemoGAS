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

	//~begin Attribute Info
	OnAttackDistanceChangedSignature.Broadcast(DemoAttributeSet->GetAttackDistance());
	OnStrengthChangedSignature.Broadcast(DemoAttributeSet->GetStrength());
	OnResilienceChangedSignature.Broadcast(DemoAttributeSet->GetResilience());
	OnIntelligenceChangedSignature.Broadcast(DemoAttributeSet->GetIntelligence());
	OnVigorChangedSignature.Broadcast(DemoAttributeSet->GetVigor());
	OnArmorChangedSignature.Broadcast(DemoAttributeSet->GetArmor());
	OnSpellArmorChangedSignature.Broadcast(DemoAttributeSet->GetSpellArmor());
	OnCriticalChanceChangedSignature.Broadcast(DemoAttributeSet->GetCriticalChance());
	OnCriticalBoncePercentChangedSignature.Broadcast(DemoAttributeSet->GetCriticalBoncePercent());
	//~end
}

void UOverlayWidgetController::BindCallBackToDependencies()
{
	UDemoAttributeSet* DemoAttributeSet = CastChecked<UDemoAttributeSet>(AttributeSet);
	//ASC提供的Attribute值改变的Delegate
	//~begin health & mana
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetHealthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)-> void
	                      {
		                      OnHealthChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetMaxHealthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)-> void
	                      {
		                      OnMaxHealthChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetManaAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)-> void
	                      {
		                      OnManaChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetMaxManaAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)-> void
	                      {
		                      OnMaxManaChangedSignature.Broadcast(Data.NewValue);
	                      });
	//end

	//~begin Attribute Info
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetAttackDistanceAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data) -> void
	                      {
		                      OnAttackDistanceChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetStrengthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data) -> void
	                      {
		                      OnStrengthChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetIntelligenceAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data) -> void
	                      {
		                      OnIntelligenceChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetResilienceAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data) -> void
	                      {
		                      OnResilienceChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetVigorAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data) -> void
	                      {
		                      OnVigorChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetArmorAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data) -> void
	                      {
		                      OnArmorChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetSpellArmorAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data) -> void
	                      {
		                      OnSpellArmorChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(DemoAttributeSet->GetCriticalChanceAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data) -> void
	                      {
		                      OnCriticalChanceChangedSignature.Broadcast(Data.NewValue);
	                      });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                      DemoAttributeSet->GetCriticalBoncePercentAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data) -> void
	                      {
		                      OnCriticalBoncePercentChangedSignature.Broadcast(Data.NewValue);
	                      });

	//~end

	//~begin
	Cast<UDemoAbilitySystemComponent>(AbilitySystemComponent)->
		OnAppliedGEToSelfAssetTagsDelegate
		.AddLambda([this](
			const FGameplayTagContainer& AssetTags)-> void
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					//通过FName获取FGameplayTag(如果有的话)
					FGameplayTag MessageTagToCheck =
						FGameplayTag::RequestGameplayTag(
							FName("Message"));
					//判断是否包含MessageTag
					if (Tag.MatchesTag(MessageTagToCheck))
					{
						const FUIWidgetRow* Row =
							GetDataTableRowByTag<FUIWidgetRow>(
								MessageDataTable,
								Tag); //需要捕获this使用
						OnReceiveMessageRowSignature.Broadcast(
							*Row);
					}
				}
			});
}
