// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Online/OnlineSessionNames.h"


UCoopSessionsSubsystem::UCoopSessionsSubsystem()
{
	bCreateServerAfterDestroy = false;
	MySessionName = "ThisIsTestSessionName";
	MapString = "";
}

void UCoopSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		FString SubSystemName = OnlineSubsystem->GetSubsystemName().ToString();
		MyPrint(SubSystemName);
		//获得OnlineSubSystem的SessionInterface
		SessionPtr = OnlineSubsystem->GetSessionInterface();
		//IOnlineSessionPtr是一个TSharedPtr 要用IsValid来判断
		if(SessionPtr.IsValid())
		{
			//绑定事件到完成Session创建 委托！！！
			//方法1：创建一个委托，绑定进去
			//FOnCreateSessionCompleteDelegate Delegate;
			//Delegate.BindUObject(this, &UCoopSessionsSubsystem::OnCreateSessionComplete);
			//FDelegateHandle Handle = SessionPtr->AddOnCreateSessionCompleteDelegate_Handle(Delegate);
			//方法2：直接绑定
			SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(
				this, &UCoopSessionsSubsystem::OnCreateSessionComplete);
			SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(
				this, &UCoopSessionsSubsystem::OnDestroySessionComplete);
			SessionPtr->OnFindSessionsCompleteDelegates.
			            AddUObject(this, &UCoopSessionsSubsystem::OnFindSessionComplete);
			SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UCoopSessionsSubsystem::OnJoinSessionComplete);
		}
	}
}

void UCoopSessionsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UCoopSessionsSubsystem::CreateServer(const FString& ServerName)
{
	if(!ServerName.IsEmpty())
	{
		FNamedOnlineSession* FNamedSessionPtr = SessionPtr->GetNamedSession(FName(MySessionName));
		//若这个SessionName对应的已经存在 先销毁再创建
		if(FNamedSessionPtr)
		{
			//FOnDestroySessionCompleteDelegate Delegate;
			//Delegate.BindUObject(this,&UCoop)
			MyPrint("Destroy Session");
			SessionPtr->DestroySession(FName(MySessionName));
			bCreateServerAfterDestroy = true;
			DestroyServerName = ServerName;
			return; //避免Async摧毁导致还没摧毁就执行下面的任务，所以直接return
		}
		MyPrint("Start Create");
		FOnlineSessionSettings Settings;
		Settings.bAllowJoinInProgress = true;
		Settings.bIsDedicated = false;
		Settings.bShouldAdvertise = true;
		Settings.NumPublicConnections = 4;
		Settings.bUseLobbiesIfAvailable = true;
		Settings.bUsesPresence = true;
		Settings.bAllowJoinViaPresence = true;
		FString SubSystemName = IOnlineSubsystem::Get()->GetSubsystemName().ToString();
		if(SubSystemName == "NULL")
		{
			Settings.bIsLANMatch = true;
		}
		else
		{
			Settings.bIsLANMatch = false;
		}
		//设定ServerName
		Settings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionPtr->CreateSession(0, FName(MySessionName), Settings);
	}
	else
	{
		ServerCreateDelegate.Broadcast(false);
	}
}


void UCoopSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	ServerCreateDelegate.Broadcast(bWasSuccessful);
	if(bWasSuccessful)
	{
		MyPrint(FString::Printf(TEXT("Create Session Name : %s"), *SessionName.ToString()));
		GetWorld()->ServerTravel(MapString + "?listen");
	}
	else
	{
		MyPrint("Create Complete Failed");
	}
}

void UCoopSessionsSubsystem::FindServer(const FString& ServerName)
{
	if(ServerName.IsEmpty())
	{
		//Empty Name Is InValid
		ServerCreateDelegate.Broadcast(false);
		return;
	}
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if("NULL" == IOnlineSubsystem::Get()->GetSubsystemName().ToString())
	{
		SessionSearch->bIsLanQuery = true;
	}
	else
	{
		SessionSearch->bIsLanQuery = false;
	}
	SessionSearch->MaxSearchResults = 9999;
	//Todo(????????)
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	JoinServerName = ServerName;
	MyPrint(FString::Printf(TEXT("Find Server %s"), *JoinServerName));
	SessionPtr->FindSessions(0, SessionSearch.ToSharedRef());
}


void UCoopSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(bCreateServerAfterDestroy)
	{
		bCreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UCoopSessionsSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	ServerJoinDelegate.Broadcast(bWasSuccessful);
	if(!bWasSuccessful)
	{
		return;
	}
	if(JoinServerName.IsEmpty())
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Complete Finding")));

	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	FOnlineSessionSearchResult* CurrentResult = nullptr;
	if(Results.Num() > 0)
	{
		for(auto Result : Results)
		{
			if(Result.IsValid())
			{
				FString CurServerName = "No-Name";
				Result.Session.SessionInfo->GetSessionId().ToString();
				Result.Session.SessionSettings.Get(FName("SERVER_NAME"), CurServerName);
				if(JoinServerName.Equals(CurServerName))
				{
					CurrentResult = &Result;
					break;
				}
			}
		}
		if(CurrentResult)
		{
			MyPrint("Join");
			SessionPtr->JoinSession(0, FName(MySessionName), *CurrentResult);
		}
		else
		{
			ServerJoinDelegate.Broadcast(bWasSuccessful);
			//没找到的逻辑
			MyPrint("No Finding Need");
		}
	}
	else
	{
		ServerJoinDelegate.Broadcast(bWasSuccessful);
		MyPrint("No result");
	}
}

void UCoopSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	ServerJoinDelegate.Broadcast(Result == EOnJoinSessionCompleteResult::Success);
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		FString Address = "";
		SessionPtr->GetResolvedConnectString(FName(MySessionName), Address);
		//UGameplayStatics::GetPlayerController(this, 0);
		APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
		MyPrint(Address);
		if(PC)
		{
			MyPrint("Client Travel Begin");
			PC->ClientTravel(Address, TRAVEL_Absolute);
			MyPrint("Client Travel End");
		}
	}
	else
	{
		//JoinFailed
		MyPrint("Failed Join");
	}
}

void UCoopSessionsSubsystem::MyPrint(FString String)
{
	UKismetSystemLibrary::PrintString(this,String);
}
