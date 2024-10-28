// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoGAS/DemoGAS.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"


//交互接口，提供一些功能包括：鼠标悬停高亮等
UINTERFACE()
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 敌人接口，提供一些功能包括：鼠标悬停高亮
 */
class DEMOGAS_API IInteractInterface
{
	GENERATED_BODY()

public:
	virtual void HighlightActor(ETeam FromTeam);
	virtual void UnHighlightActor();
};
