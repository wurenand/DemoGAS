
#include "DemoGAS/Public/Character/DemoCharacterBase.h"

#include "AbilitySystemComponent.h"


ADemoCharacterBase::ADemoCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//Weapon初始化
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(),FName(TEXT("WeaponHandSocket")));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

UAbilitySystemComponent* ADemoCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ADemoCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void ADemoCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADemoCharacterBase::InitialASCActorInfo()
{
	
}

void ADemoCharacterBase::InitialPrimaryAttributes() const
{
	if(DefaultPrimaryAttributesEffectClass == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("未设置初始化Attributes的GE,ADemoCharacterBase"));
		return;
	}
	check(IsValid(GetAbilitySystemComponent()));
	
	FGameplayEffectContextHandle GEContentHandle = GetAbilitySystemComponent()->MakeEffectContext();
	GEContentHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributesEffectClass,1,GEContentHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(),GetAbilitySystemComponent());
}




