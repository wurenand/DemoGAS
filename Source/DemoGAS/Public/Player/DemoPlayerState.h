// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DemoGAS/DemoGAS.h"
#include "GameFramework/PlayerState.h"
#include "DemoPlayerState.generated.h"

class ULevelUpInfo;class UAbilitySystemComponent;
class UAttributeSet;

//PS中一个参数变化的通用Delegate
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateParamChanged, int32);

/**
 * 在PlayerState中给玩家添加AbilitySystemComponent和AttributeSet
 * 这样可以在玩家死亡时依然保留ASC和属性集
 * 但是对于AI，可以直接在Character上创建ASC和属性集
 */
UCLASS()
class DEMOGAS_API ADemoPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	//一些注释可以参考DemoCharacterBase
public:
	ADemoPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//外部接口提供AS和ASC
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	//~begin Level XP
	//封装后的接口
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	//下面几个函数应该只在Server调用
	void SetXP(int32 InXP);
	UFUNCTION(BlueprintCallable)
	void AddXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void AddLevel(int32 InLevel);
	
	FOnPlayerStateParamChanged OnXPChangedDelegate;
	FOnPlayerStateParamChanged OnLevelChangedDelegate;

	//填入升级配置的DataAsset
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Level")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	//~end
	
	
	//玩家所属阵营
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Team, Category = "Team")
	ETeam Team = ETeam::ETeam_Red;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level, Category = "Level")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP, Category = "Level")
	int32 XP = 0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	UFUNCTION()
	void OnRep_Team();
	UFUNCTION()
	void OnRep_XP();
};
