#pragma once

#include "CoreMinimal.h"
#include "DemoInputConfig.h"
#include "EnhancedInputComponent.h"
#include "DemoInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
/**
 * 自定义InputComponent，提供函数来支持动态绑定函数到IA上
 */
class DEMOGAS_API UDemoInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UDemoInputComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//模板绑定函数
	template <typename UserClass, typename PressesFuncType, typename ReleaseFuncType, typename TriggeredFuncType>
	void BindAbilityActions(const UDemoInputConfig* InputConfig, UserClass* Object, PressesFuncType PressesType,
	                        ReleaseFuncType ReleaseType,
	                        TriggeredFuncType TriggeredType);

protected:
	virtual void BeginPlay() override;
};

template <typename UserClass, typename PressesdFuncType, typename ReleasedFuncType, typename TriggeredFuncType>
void UDemoInputComponent::BindAbilityActions(const UDemoInputConfig* InputConfig, UserClass* Object,
	PressesdFuncType PressesType, ReleasedFuncType ReleaseType, TriggeredFuncType TriggeredType)
{
	check(InputConfig);
	for(const FDemoInputAction& Action : InputConfig->AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTag.IsValid())
		{
			//给所有InputAction都绑定一个函数，但是传入的可变参数InputTag不同，根据这个Tag来区分触发的Action
			if(PressesType)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,PressesType,Action.InputTag);
			}
			if(ReleaseType)
			{
				BindAction(Action.InputAction,ETriggerEvent::Completed,Object,ReleaseType,Action.InputTag);
			}
			if(TriggeredType)
			{
				BindAction(Action.InputAction,ETriggerEvent::Triggered,Object,TriggeredType,Action.InputTag);
			}
		}
	}
}
