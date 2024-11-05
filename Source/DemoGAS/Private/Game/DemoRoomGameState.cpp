// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DemoRoomGameState.h"

#include "Net/UnrealNetwork.h"

void ADemoRoomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADemoRoomGameState,Players);
}

void ADemoRoomGameState::OnRep_Players()
{
	//TODO:玩家加入
}
