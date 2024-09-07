// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/DemoAbilitySystemComponent.h"


UDemoAbilitySystemComponent::UDemoAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDemoAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDemoAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

