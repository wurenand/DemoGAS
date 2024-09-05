
#include "DemoGAS/Public/Character/DemoCharacterBase.h"


ADemoCharacterBase::ADemoCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//Weapon初始化
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(),FName(TEXT("WeaponHandSocket")));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ADemoCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}
