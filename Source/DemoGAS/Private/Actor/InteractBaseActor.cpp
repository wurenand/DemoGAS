
#include "Actor/InteractBaseActor.h"

#include "Net/UnrealNetwork.h"


AInteractBaseActor::AInteractBaseActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInteractBaseActor::SetTeam(ETeam InTeam)
{
	Team = InTeam;
}

void AInteractBaseActor::SetTargetActor(const ADemoCharacterBase* InTargetActor)
{
	TargetActor = InTargetActor;
}

void AInteractBaseActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInteractBaseActor, TargetActor);
}

void AInteractBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}



