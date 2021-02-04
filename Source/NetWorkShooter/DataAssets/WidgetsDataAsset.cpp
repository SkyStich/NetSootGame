// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetsDataAsset.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "NetWorkShooter/SingletonClass/GameSingletonClass.h"

UUserWidget* UWidgetsDataAsset::SyncCreateWidget(UObject* WorldContextObject, TSoftClassPtr<UUserWidget> Asset, APlayerController* OwningPlayer)
{
	if(Asset.IsNull())
	{
		FString const InstigatorName = WorldContextObject != nullptr ? WorldContextObject->GetFullName() : TEXT("Unknown");
		UE_LOG(LogAssetData, Warning, TEXT("UWidgetsDataAsset::SyncCreateWidget -- Asset ptr is null for: %s"), *InstigatorName);
		return nullptr;
	}
	
	/** Load asset into memory (sync) */
	FStreamableManager& StreamManager = UGameSingletonClass::Get().AssetLoader;
	FSoftObjectPath const Ref = Asset.ToSoftObjectPath();
	StreamManager.LoadSynchronous(Asset);

	UClass* WidgetType = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *Ref.ToString()));
	if(!WidgetType)
	{
		return nullptr;
	}
	
	UUserWidget* UserWidget = nullptr;
	if(OwningPlayer == nullptr)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
		UserWidget = CreateWidget<UUserWidget>(World, WidgetType);
	}
	else
	{
		UserWidget = CreateWidget<UUserWidget>(OwningPlayer, WidgetType);
	}

	if(UserWidget)
	{
		UserWidget->SetFlags(RF_StrongRefOnFrame);
	}
	return UserWidget;
}