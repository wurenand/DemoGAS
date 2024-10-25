// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Library/DemoSystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "DemoAbilitySystemTypes.h"
#include "Character/DemoCharacterBase.h"
#include "Game/DemoGameModeBase.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"

void UDemoSystemLibrary::InitialDefaultAttributes(const UObject* WorldContentObject, ECharacterClass CharacterClass,
                                                  float Level, ADemoCharacterBase* CharacterBase)
{
	//将存放在GM中的CharacterClass DataAsset拿出来
	ADemoGameModeBase* DemoGameModeBase = Cast<ADemoGameModeBase>(UGameplayStatics::GetGameMode(WorldContentObject));
	if (DemoGameModeBase == nullptr)
	{
		return;
	}

	checkf(DemoGameModeBase->CharacterClassInfo, TEXT("没有设置GM中的CharacterClassInfo"))

	FCharacterClassDefaultInfo CharacterClassInfo = DemoGameModeBase->CharacterClassInfo->GetClassDefaultInfo(
		CharacterClass);

	//Apply GE
	CharacterBase->ApplyEffectToSelf(CharacterClassInfo.PrimaryAttributesEffectClass, Level);
	if (!CharacterBase->bFirstInitial)
	{
		CharacterBase->ApplyEffectToSelf(DemoGameModeBase->CharacterClassInfo->VitalAttributesEffectClass, Level);
		CharacterBase->bFirstInitial = true;
	}
	//TODO:这里可以加一个else用作升级时Health的更改，转移CharacterBase是否存活？
}

void UDemoSystemLibrary::GiveStateAbilities(const UObject* WorldContentObject, ADemoCharacterBase* CharacterBase)
{
	//添加用于实现状态的Ability
	ADemoGameModeBase* DemoGameModeBase = Cast<ADemoGameModeBase>(UGameplayStatics::GetGameMode(WorldContentObject));
	if (DemoGameModeBase == nullptr)
	{
		return;
	}
	checkf(DemoGameModeBase->CharacterClassInfo, TEXT("没有设置GM中的CharacterClassInfo"))
	for (TSubclassOf<UGameplayAbility> StateAbilityClass : DemoGameModeBase->CharacterClassInfo->StateAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(StateAbilityClass);
		CharacterBase->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}

void UDemoSystemLibrary::GivePassiveAbilitiesAndActive(const UObject* WorldContentObject,
                                                       ADemoCharacterBase* CharacterBase)
{
	//添加用于实现状态的Ability
	ADemoGameModeBase* DemoGameModeBase = Cast<ADemoGameModeBase>(UGameplayStatics::GetGameMode(WorldContentObject));
	if (DemoGameModeBase == nullptr)
	{
		return;
	}
	checkf(DemoGameModeBase->CharacterClassInfo, TEXT("没有设置GM中的CharacterClassInfo"))
	for (TSubclassOf<UGameplayAbility> PassiveAbility : DemoGameModeBase->CharacterClassInfo->PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(PassiveAbility);
		CharacterBase->GetAbilitySystemComponent()->GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UDemoSystemLibrary::GiveStartUpAbilitiesToCharacter(const UObject* WorldContentObject, ADemoCharacterBase* CharacterBase)
{
	ADemoGameModeBase* DemoGameModeBase = Cast<ADemoGameModeBase>(UGameplayStatics::GetGameMode(WorldContentObject));
	if (DemoGameModeBase)
	{
		FCharacterClassDefaultInfo CharacterClassInfo = DemoGameModeBase->CharacterClassInfo->GetClassDefaultInfo(
			CharacterBase->CharacterClass);
		Cast<UDemoAbilitySystemComponent>(CharacterBase->GetAbilitySystemComponent())->AddAbilitiesToCharacter(
			{
				CharacterClassInfo.NormalAttackClass,
				CharacterClassInfo.PassiveAbilityClass
			});
	}
}

bool UDemoSystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FDemoGameplayEffectContext* DemoContext = static_cast<const FDemoGameplayEffectContext*>(ContextHandle
		.
		Get()))
	{
		return DemoContext->IsCriticalHit();
	}
	return false;
}

void UDemoSystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle, bool bIsCriticalHit)
{
	if (FDemoGameplayEffectContext* DemoContext = static_cast<FDemoGameplayEffectContext*>(ContextHandle.Get()))
	{
		DemoContext->SetIsCriticalHit(bIsCriticalHit);
	}
}
