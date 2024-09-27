// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Abilities/ProjectileSpellBase.h"

#include "Actor/DemoProjectile.h"
#include "Character/DemoCharacterBase.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UProjectileSpellBase::SpawnOneProjectile(FVector TargetLocation,
                                              FVector OverrideSpawnLocation, bool bUseOverrideSpawnLoc)
{
	//Server Check，只在Server生成，通过Replication到Client

	//TODO:Client目前不会生成
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer)
	{
		return;
	}

	//使用延迟生成用于为投射物设置一些参数或信息

	//******位置
	//通过ICombat接口，获取当前这个InputTag的GA的投射物生成位置
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	FVector SpawnLocation = CombatInterface->GetCombatSocketLocation(TriggerInputTag);

	if(bUseOverrideSpawnLoc)
	{
		SpawnLocation = OverrideSpawnLocation;
	}

	//统一高度，避免打不到人
	TargetLocation.Z = 50.f;
	SpawnLocation.Z = 50.f;

	//计算角度
	FRotator TargetRot = (TargetLocation - SpawnLocation).ToOrientationRotator();

	FTransform SpawnTransform(TargetRot, SpawnLocation, FVector::One());


	AActor* GAOwnerActor = GetOwningActorFromActorInfo();
	ADemoProjectile* DemoProjectile = GetWorld()->SpawnActorDeferred<ADemoProjectile>(
		ProjectileClass,
		SpawnTransform,
		GAOwnerActor,
		Cast<APawn>(GAOwnerActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	//*****信息
	const ADemoCharacterBase* DemoCharacterBase = Cast<ADemoCharacterBase>(GetAvatarActorFromActorInfo());
	if(DemoCharacterBase)
	{
		DemoProjectile->SetTeam(DemoCharacterBase->Team);
		DemoProjectile->SetTargetActor(nullptr); //TODO:这里根据需要，通过SpawnOneProjectile的参数传入，暂时不处理
	}


	//TODO:给Projectile一个GE Spec来施加Damage


	DemoProjectile->FinishSpawning(SpawnTransform);
}

void UProjectileSpellBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//TODO:为什么从Client激活会激活两次？
	UKismetSystemLibrary::PrintString(this, FString(TEXT("C++ 激活")));
}
