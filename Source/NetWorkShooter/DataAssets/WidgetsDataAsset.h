// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WidgetsDataAsset.generated.h"

UENUM(BlueprintType)
enum EWidgetType
{
	EveryManForHimSelfMode,
	CommandMode
};

USTRUCT(BlueprintType)
struct FWidgetData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UUserWidget> TabMenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UUserWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UUserWidget> PreStartWidgetClass;
};

UCLASS()
class NETWORKSHOOTER_API UWidgetsDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UUserWidget* SyncCreateWidget(UObject* WorldContextObject, TSoftClassPtr<UUserWidget> Asset, APlayerController* OwningPlayer);
	FWidgetData* GetWidgetData() { return &WidgetData; }
private:

	UPROPERTY(EditAnywhere)
	FWidgetData WidgetData;
};
