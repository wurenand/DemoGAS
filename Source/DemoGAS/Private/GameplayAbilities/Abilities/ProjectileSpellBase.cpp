// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilities/Abilities/ProjectileSpellBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "Actor/DemoProjectile.h"
#include "Character/DemoCharacterBase.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UProjectileSpellBase::SpawnOneProjectile(FVector TargetLocation,
                                              FVector OverrideSpawnLocation, bool bUseOverrideSpawnLoc,
                                              ADemoCharacterBase* InTargetActor)
{
	//Server Check，只在Server生成，通过Replication到Client
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

	//*****效果对象和方式
	ADemoCharacterBase* DemoCharacterBase = Cast<ADemoCharacterBase>(GetAvatarActorFromActorInfo());
	if(DemoCharacterBase)
	{
		DemoProjectile->SetTeam(DemoCharacterBase->GetTeam());
		DemoProjectile->SetTargetActor(InTargetActor);
	}

	//创建DamageEffectSpec,用于碰撞后施加GE
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

	//填写EffectContext
	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(DemoProjectile);
	
	
	//生成Spec
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),ContextHandle);
	
	//获取当前等级的伤害值（基础的）
	const float ScaledDamage = DamageValue.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,UGameplayTagsManager::Get().RequestGameplayTag(FName("DamageCaller")),ScaledDamage);
	DemoProjectile->DamageEffectSpecHandle = SpecHandle;
	

	DemoProjectile->FinishSpawning(SpawnTransform);
}

void UProjectileSpellBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//Client会先本地activate，并通知Server Activate
}
