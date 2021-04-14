// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "WebRequestBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWebRequest, Log, All);

UENUM(BlueprintType)
enum class EWebRequestType : uint8
{
	Post,
	Get
};

/**
 * Base class 
 */
UCLASS()
class NETWORKSHOOTER_API UWebRequestBase : public UObject
{
	GENERATED_BODY()

	void InitRequest(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request, const FString& RequestType, const FString& ScriptURL);

protected:

	/** принимает успешные ответы от веб сервера */
	virtual void CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse) {}

	/** Принимает не успешные ответы от веб сервера */
	virtual void CallJsonFail() {}

	virtual void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessFull);

	bool CallWebScript(const FString& ScriptURL, TSharedPtr<FJsonObject>& JsonRequest, EWebRequestType RequestType = EWebRequestType::Post);

	TSharedPtr<FJsonObject> CreateJsonRequest();

private:

	FHttpModule* Http;
	FString JsonStream = "JsonStream";

	bool bUsingSSL = false;
};
