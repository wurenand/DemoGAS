#include "DemoGAS/Public/Character/DemoCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "Components/CapsuleComponent.h"
#include "DemoGAS/DemoGAS.h"
#include "Game/DemoGameModeBase.h"
#include "GameplayAbilities/DemoAbilitySystemComponent.h"
#include "GameplayAbilities/Library/DemoSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


ADemoCharacterBase::ADemoCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//设定后PC中的CursorTrace才能检测到
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	//能够与投射物，范围等交互
	GetMesh()->SetCollisionResponseToChannel(ECC_Ability, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true); //必须打开，不然不能碰撞

	//Weapon初始化
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName(TEXT("WeaponHandSocket")));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	//关闭摄像机通道
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}


void ADemoCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADemoCharacterBase, Team);
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
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString("Highlight"));
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

UAnimMontage* ADemoCharacterBase::GetStunnedMontage_Implementation()
{
	return StunnedMontage;
}

void ADemoCharacterBase::Die()
{
	MulticastHandleDeath();
	
	UKismetSystemLibrary::Delay(this,5.f,FLatentActionInfo());
	Destroy();
}

void ADemoCharacterBase::MulticastHandleDeath_Implementation()
{
	//可以在这设置布娃娃系统等...
}


void ADemoCharacterBase::InitialAbilitySystem()
{
	//子类实现
}

void ADemoCharacterBase::RegisterStateEvent()
{
	if(AbilitySystemComponent)
	{
		//TODO:AddUObject绑定虚函数是如何实现子类就调用子类重写的函数的？
		AbilitySystemComponent->RegisterGameplayTagEvent(
			UGameplayTagsManager::Get().RequestGameplayTag(FName("State.Stunned")),
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StateCallback);
	}
}

void ADemoCharacterBase::StateCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if(!HasAuthority()) return;
	FGameplayTagContainer Container;
	Container.AddTag(CallbackTag);
	if(NewCount == 0)
	{
		//TODO:这里要当State的Tag消失后手动EndAbility  暂时在GA中用Delay定时End吧
	}
	else
	{
		GetAbilitySystemComponent()->TryActivateAbilitiesByTag(Container);
	}
	UE_LOG(LogTemp, Warning, TEXT("FatherState %s"), *CallbackTag.ToString());
}

void ADemoCharacterBase::InitialDefaultAttributes() const
{
	UDemoSystemLibrary::InitialDefaultAttributes(this, CharacterClass, 1, const_cast<ADemoCharacterBase*>(this));
}

void ADemoCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& EffectClassToBeApplied,
                                           float Level) const
{
	if(EffectClassToBeApplied == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("传入的Class为nullptr ADemoCharacterBase"));
		return;
	}
	check(IsValid(GetAbilitySystemComponent()));

	FGameplayEffectContextHandle GEContentHandle = GetAbilitySystemComponent()->MakeEffectContext();
	GEContentHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(
		EffectClassToBeApplied, Level, GEContentHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), GetAbilitySystemComponent());
}

void ADemoCharacterBase::AddAbilityToCharacter()
{
	if(!HasAuthority())
	{
		return;
	}
	UDemoAbilitySystemComponent* DemoAbilitySystemComponent = Cast<UDemoAbilitySystemComponent>(AbilitySystemComponent);
	//DemoAbilitySystemComponent->AddAbilitiesToCharacter(StartUpAbilities); 暂时隐藏掉
	ADemoGameModeBase* DemoGameModeBase = Cast<ADemoGameModeBase>(UGameplayStatics::GetGameMode(this));
	if(DemoGameModeBase)
	{
		FCharacterClassDefaultInfo CharacterClassInfo = DemoGameModeBase->CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
		TArray<TSubclassOf<UGameplayAbility>> GAs{CharacterClassInfo.NormalAttackClass};
		GAs.Append(CharacterClassInfo.AbilitiesClass);
		DemoAbilitySystemComponent->AddAbilitiesToCharacter(GAs);
	}
	

	
}

void ADemoCharacterBase::OnRep_Team()
{
}
