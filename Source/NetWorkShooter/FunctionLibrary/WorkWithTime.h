// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WorkWithTime.generated.h"

/**
 * library for work with time
 */
UCLASS(meta=(BlueprintThreadSafe, ScriptName = "TimeLibrary"))
class NETWORKSHOOTER_API UWorkWithTime : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** return string in format Min:Sec (00:00) */
	UFUNCTION(BlueprintPure, Category = "TimeLibrary")
	static FString GetTimeByStandard(int32 Min, int32 Sec);

	/** return string in format Min:Sec (00:00) */
	UFUNCTION(BlueprintPure, Category = "TimeLibrary")
	static FString ConvertSecondOnTimeStandard(int32 Sec);

	/** Get minutes from total seconds and remainder seconds */
	UFUNCTION(BlueprintPure, Category = "TimeLibrary")
	static void ConvertTotalSecondInStandardTime(int32 const Sec, int32 & ReturnSeconds, int32 & ReturnMinutes);
};
