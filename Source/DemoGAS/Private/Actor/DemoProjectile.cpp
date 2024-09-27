#include "Actor/DemoProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ADemoProjectile::ADemoProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	//只在Server生成，复制给Client
	SetReplicates(true);
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ADemoProjectile::Destroyed()
{
	if(!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactNiagara,GetActorLocation());
	}
	
	Super::Destroyed();
}

void ADemoProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::ADemoProjectile::OnSphereOverlappedBegin);
}

void ADemoProjectile::OnSphereOverlappedBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:这里应该根据需要，区分情况：(这些参数是否应该放在InteractBaseActor?)例如，攻击对象为敌对还是队友，同时还应该区分追踪还是定向。修改在ProjectileSpellBase

	//暂时只处理一种情况了
	
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactNiagara,GetActorLocation());

	if(HasAuthority())
	{
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

