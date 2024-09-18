// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DemoInputConfig.h"

const UInputAction* UDemoInputConfig::FindInputActionFromTag(const FGameplayTag& Tag, bool bLogNotFound)
{
	for(const FDemoInputAction& DemoInputAction : AbilityInputActions)
	{
		if(DemoInputAction.InputAction && DemoInputAction.InputTag == Tag)
		{
			return DemoInputAction.InputAction;
		}
	}
	
	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Log, TEXT("Tag[%s] Not Found In InputConfig"), *Tag.GetTagName().ToString());
	}
	return nullptr;
}
