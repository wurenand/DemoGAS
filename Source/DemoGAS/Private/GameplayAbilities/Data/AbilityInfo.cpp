// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Data/AbilityInfo.h"

#include "DemoGAS/DemoLogChannels.h"

FDemoAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound)
{
	for(const FDemoAbilityInfo& AbilityInfo : AbilityInfos)
	{
		if(AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AbilityInfo;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogDemoGAS, Error, TEXT("Not Found Ability Info"));
	}
	return FDemoAbilityInfo();
}
