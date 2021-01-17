// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StreamableManager.h"
#include "GameSingletonClass.generated.h"

UCLASS(Config=Game, NotPlaceable)
class NETWORKSHOOTER_API UGameSingletonClass : public UObject, public FTickerObjectBase
{
	GENERATED_BODY()

	UGameSingletonClass();

public:

	static UGameSingletonClass& Get();
	virtual bool Tick(float DeltaTime) override;

public:
	
	FStreamableManager AssetLoader;
	
};
