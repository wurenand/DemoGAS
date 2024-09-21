
#include "DemoGAS/Public/Character/DemoCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "DemoGAS/DemoGAS.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"


ADemoCharacterBase::ADemoCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//设定后PC中的CursorTrace才能检测到
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
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

void ADemoCharacterBase::HighlightActor(ETeam FromTeam)
{
	//理由后期处理材质，实现描边（一定要在项目设置里面打开 自定义深度的 启动模板）

	if(FromTeam == Team)
	{
		GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_WHITE);
		WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_WHITE);
	}
	else
	{
		GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
		WeaponMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
	GetMesh()->SetRenderCustomDepth(true);
	WeaponMesh->SetRenderCustomDepth(true);
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,FString("Highlight"));

}

void ADemoCharacterBase::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	WeaponMesh->SetRenderCustomDepth(false);
}

ETeam ADemoCharacterBase::GetTeam()
{
	return Team;
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

void ADemoCharacterBase::AddAbilityToCharacter()
{
	if(!HasAuthority()) return;
	UDemoAbilitySystemComponent* DemoAbilitySystemComponent = Cast<UDemoAbilitySystemComponent>(AbilitySystemComponent);
	DemoAbilitySystemComponent->AddAbilitiesToCharacter(StartUpAbilities);
}




