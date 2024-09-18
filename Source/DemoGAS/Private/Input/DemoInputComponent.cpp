#include "Input/DemoInputComponent.h"


UDemoInputComponent::UDemoInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UDemoInputComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}


void UDemoInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

