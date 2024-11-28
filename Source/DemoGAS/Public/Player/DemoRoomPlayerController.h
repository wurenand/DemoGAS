// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DemoRoomPlayerController.generated.h"

enum class ECharacterClass : uint8;
enum class ETeam : uint8;
/**
 * 作为Room关卡中的PC，控制RoomUI
 */
UCLASS()
class DEMOGAS_API ADemoRoomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,Server,Reliable,Category = "Select")
	void SelectTeam(ETeam InTeam);

	UFUNCTION(BlueprintCallable,Server,Reliable,Category = "Select")
	void SelectHero(ECharacterClass InCharacterClass);

	UFUNCTION(BlueprintCallable,Server,Reliable,Category = "Ready")
	void SetReady(bool bIsReady);
	
protected:
	UPROPERTY(EditAnywhere, Category = "RoomUI")
	TSubclassOf<UUserWidget> RoomWidgetClass;
	UPROPERTY(EditAnywhere, Category = "RoomUI")
	TObjectPtr<UUserWidget> RoomWidget;
};
