// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Library/DemoSystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "DemoAbilitySystemTypes.h"
#include "Character/DemoCharacterBase.h"
#include "Game/DemoGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UDemoSystemLibrary::InitialDefaultAttributes(const UObject* WorldContentObject, ECharacterClass CharacterClass,
                                                  float Level, ADemoCharacterBase* CharacterBase)
{
	//将存放在GM中的CharacterClass DataAsset拿出来
	ADemoGameModeBase* DemoGameModeBase = Cast<ADemoGameModeBase>(UGameplayStatics::GetGameMode(WorldContentObject));
	if(DemoGameModeBase == nullptr)
	{
		return;
	}

	checkf(DemoGameModeBase->CharacterClassInfo,TEXT("没有设置GM中的CharacterClassInfo"))
	
	FCharacterClassDefaultInfo CharacterClassInfo = DemoGameModeBase->CharacterClassInfo->GetClassDefaultInfo(
		CharacterClass);

	//Apply GE
	CharacterBase->ApplyEffectToSelf(CharacterClassInfo.PrimaryAttributesEffectClass,Level);
	CharacterBase->ApplyEffectToSelf(DemoGameModeBase->CharacterClassInfo->SecondaryAttributesEffectClass,Level);
	CharacterBase->ApplyEffectToSelf(DemoGameModeBase->CharacterClassInfo->VitalAttributesEffectClass,Level);
}

void UDemoSystemLibrary::GiveStateAbilities(const UObject* WorldContentObject, ADemoCharacterBase* CharacterBase)
{
	//添加用于实现状态的Ability
	ADemoGameModeBase* DemoGameModeBase = Cast<ADemoGameModeBase>(UGameplayStatics::GetGameMode(WorldContentObject));
	if(DemoGameModeBase == nullptr)
	{
		return;
	}
	checkf(DemoGameModeBase->CharacterClassInfo,TEXT("没有设置GM中的CharacterClassInfo"))
	for(TSubclassOf<UGameplayAbility> StateAbilityClass : DemoGameModeBase->CharacterClassInfo->StateAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(StateAbilityClass);
		CharacterBase->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
	
}

bool UDemoSystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if(const FDemoGameplayEffectContext* DemoContext = static_cast<const FDemoGameplayEffectContext*>(ContextHandle.Get()))
	{
		return DemoContext->IsCriticalHit();
	}
	return false;
}

void UDemoSystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& ContextHandle, bool bIsCriticalHit)
{
	if(FDemoGameplayEffectContext* DemoContext = static_cast<FDemoGameplayEffectContext*>(ContextHandle.Get()))
	{
		DemoContext->SetIsCriticalHit(bIsCriticalHit);
	}
}
