// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AccountInfo.generated.h"

USTRUCT(BlueprintType)
struct FAccountInfo
{
	GENERATED_BODY()

	FString GetLogin() const { return Login; }
	FString GetPassword() const { return Password; }

private:

	FString Login;
	FString Password;
};