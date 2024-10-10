#pragma once

#include "GameplayEffectTypes.h"

#include "DemoAbilitySystemTypes.generated.h"
/**
 * 自定义子类EffectContext
 * 传递一些信息
 * 子类必须要：1 GetScriptStruct 2 NetSerialize 3 TStructOpsTypeTraits<FDemoGameplayEffectContext>
 */
USTRUCT(BlueprintType)
struct FDemoGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	bool IsCriticalHit() const {return bIsCriticalHit;};
	void SetIsCriticalHit(bool bInCriticalHit) {bIsCriticalHit = bInCriticalHit;}; 
	
	//子类必须重写 （类似UClass的GetClass函数）
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayEffectContext::GetScriptStruct();
	}

	
	/**
	 * 
	 * @param Ar 能够序列化和解析序列化的成员变量
	 * @param Map 通过object或name映射到索引或从索引映射 （用于解析序列化后的数据具体的位置？）
	 * @param bOutSuccess 
	 * @return 
	 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	bool bIsCriticalHit = false;
};

template<>
struct TStructOpsTypeTraits<FDemoGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FDemoGameplayEffectContext>
{
	enum
	{
		WithNetSerialize = true,
		WithCopy = true,
	};
};