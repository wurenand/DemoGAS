
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageWidgetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEMOGAS_API UDamageWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetDamageText(float Damage,bool bIsCriticalHit);
};
