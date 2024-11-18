// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CoopSessionsSubsystem.generated.h"


//声明自定义委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerCreateDelegate, bool , bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerJoinDelegate, bool, bWasSuccessful);

/**
 * Designed To 管理多人联机会话
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UCoopSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCoopSessionsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void CreateServer(const FString& ServerName);
	UFUNCTION(BlueprintCallable)
	void FindServer(const FString& ServerName);

	IOnlineSessionPtr SessionPtr;

protected:
	
	
	//SessionName与ServerName不同
	//SessionName是针对主机的
	//ServerName是针对创建的房间的
	//做到一台主机只能同时创建一个房间，且别人创建同名ServerName不会挤掉（但是应该不能创建吧...TODO）
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void MyPrint(FString String);
	void OnFindSessionComplete(bool bWasSuccessful);
	//下面的两个变量是用来去除重复SessionName 再创建新Session的
	bool bCreateServerAfterDestroy; //避免正常销毁Session后，重新调用Create
	FString DestroyServerName;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FString JoinServerName;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString MySessionName;

	UPROPERTY(BlueprintAssignable)
	FServerCreateDelegate ServerCreateDelegate;

	UPROPERTY(BlueprintAssignable)
	FServerJoinDelegate ServerJoinDelegate;

	UPROPERTY(BlueprintReadWrite)
	FString MapString;
	
};
