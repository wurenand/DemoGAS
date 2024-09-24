// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Abilities/ProjectileSpellBase.h"

#include "Actor/DemoProjectile.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UProjectileSpellBase::SpawnOneProjectile(bool bUseOverrideTrans , FTransform OverrideTransform )
{
	//Server Check，只在Server生成，通过Replication到Client
	//TODO:Client目前不会生成
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer)
	{
		return;
	}

	//使用延迟生成用于为投射物设置一些参数或信息
	//通过ICombat接口，获取当前这个InputTag的GA的投射物生成位置
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	FVector SpawnLocation = CombatInterface->GetCombatSocketLocation(TriggerInputTag);
	FTransform SpawnTransform(GetAvatarActorFromActorInfo()->GetActorRotation(), SpawnLocation, FVector::One());

	if(bUseOverrideTrans)
	{
		SpawnTransform = OverrideTransform;
	}
	
	AActor* GAOwnerActor = GetOwningActorFromActorInfo();
	ADemoProjectile* DemoProjectile = GetWorld()->SpawnActorDeferred<ADemoProjectile>(
		ProjectileClass,
		SpawnTransform,
		GAOwnerActor,
		Cast<APawn>(GAOwnerActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

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
