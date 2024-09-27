
#include "Actor/InteractBaseActor.h"


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

void AInteractBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}



