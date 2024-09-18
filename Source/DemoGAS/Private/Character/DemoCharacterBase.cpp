
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

void ADemoCharacterBase::InitialAbilitySystem()
{
	//子类实现
}

void ADemoCharacterBase::InitialDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributesEffectClass,1);
	ApplyEffectToSelf(SecondaryAttributesUpdateEffectClass,1);
	ApplyEffectToSelf(InitialVitalAttributesEffectClass,1); //要在MaxHealth和MaxMana设置之后调用s
}

void ADemoCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClassToBeApplied, float Level) const
{
	if(EffectClassToBeApplied == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("传入的Class为nullptr ADemoCharacterBase"));
		return;
	}
	check(IsValid(GetAbilitySystemComponent()));
	
	FGameplayEffectContextHandle GEContentHandle = GetAbilitySystemComponent()->MakeEffectContext();
	GEContentHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClassToBeApplied,Level,GEContentHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(),GetAbilitySystemComponent());
}




