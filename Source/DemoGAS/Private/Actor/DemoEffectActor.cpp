// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DemoEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "GameplayAbilities/DemoAttributeSet.h"


ADemoEffectActor::ADemoEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
	InteractSphere->SetupAttachment(Mesh);
}

void ADemoEffectActor::BeginPlay()
{
	Super::BeginPlay();

	//绑定重叠事件
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this,&ADemoEffectActor::OnBeginOverlap);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this,&ADemoEffectActor::OnEndOverlap);
}


void ADemoEffectActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:应该使用GE来修改，但是目前先用const_cast替代
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor);
	if(ASI)
	{
		//在OwnerActor的构造方法中创建的AttributeSet将会自动注册到ASC。这一步必须在C++中完成
		//所以可以通过ASC的GetAttributeSet函数获取
		const UAttributeSet* AttributeSet = ASI->GetAbilitySystemComponent()->GetAttributeSet(UDemoAttributeSet::StaticClass());
		//const不可以修改成员，用const_cast转换
		UAttributeSet* MutableAttributeSet = const_cast<UAttributeSet*>(AttributeSet);
		UDemoAttributeSet* DemoAttributeSet = Cast<UDemoAttributeSet>(MutableAttributeSet);
		DemoAttributeSet->SetHealth(DemoAttributeSet->GetHealth() + 25.f);
		Destroy();
	}
}

void ADemoEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}



