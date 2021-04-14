// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WebRequestBase.h"
#include "WebRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResultTest, const FString&,  ValueString, float, ValueFloat);

USTRUCT()
struct FKeyNumber
{
	GENERATED_BODY()
	
public:

	FKeyNumber() : Key(""), Value(-1.f) {}
	FKeyNumber(const FString & NewKey, const float& NewValue) :  Key(NewKey), Value(NewValue) {}

	UPROPERTY()
	FString Key;

	UPROPERTY()
	float Value;
};

USTRUCT()
struct FKeyString
{
	GENERATED_BODY()

public:

	FKeyString() : Key(""), Value("") {}
	FKeyString(const FString& NewKey, const FString& NewValue) :  Key(NewKey), Value(NewValue) {}

	UPROPERTY()
	FString Key;

	UPROPERTY()
	FString Value;
};

UCLASS(BlueprintType)
class NETWORKSHOOTER_API UWebRequest : public UWebRequestBase
{
	GENERATED_BODY()

protected:

	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) override;
	virtual void CallJsonFail() override {}

public:

	UFUNCTION(BlueprintCallable, Category = "WebRequest", meta = (DisplayName = "AddNumber (BP)"))
	void AddNumber(const FString& Key, const float Value) { KeyNumbers.Add(FKeyNumber(Key, Value)); }

	UFUNCTION(BlueprintCallable, Category = "WebRequest", meta = (DisplayName = "AddString (BP)"))
	void AddString(const FString& Key, const FString& Value) { KeyStrings.Add(FKeyString(Key, Value)); }

	UFUNCTION(BlueprintCallable, Category = "WebRequest", meta = (DisplayName = "Call web script"))
	void CallScript(const FString& ScriptURL);

private:

	UPROPERTY()
	TArray<FKeyNumber>KeyNumbers;

	UPROPERTY()
	TArray<FKeyString>KeyStrings;

public:

	UPROPERTY(BlueprintAssignable, Category = "WebRequest|Delegates")
	FResultTest OnResultTest;
};
