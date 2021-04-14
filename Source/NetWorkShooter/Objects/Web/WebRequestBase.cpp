// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequestBase.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

DEFINE_LOG_CATEGORY(LogWebRequest);

TSharedPtr<FJsonObject> UWebRequestBase::CreateJsonRequest()
{
	return MakeShareable(new FJsonObject);
}

void UWebRequestBase::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessFull)
{
	if(bSuccessFull)
	{
		TSharedPtr<FJsonObject> JsonObject;

		TSharedRef<TJsonReader<TCHAR>>Reader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());

		UE_LOG(LogWebRequest, Log, TEXT("OnResponseReceived response Json: \n%s"), *Response->GetContentAsString());

		if(FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			CallJsonResponse(JsonObject);
		}
		else
		{
			UE_LOG(LogWebRequest, Error, TEXT("OnResponseReceived Fail to deserialize json!"));
			CallJsonFail();
		}
	}
	else
	{
		CallJsonFail();
	}
}

void UWebRequestBase::InitRequest(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request, const FString& RequestType, const FString& ScriptURL)
{
	Request->OnProcessRequestComplete().BindUObject(this, &UWebRequestBase::OnResponseReceived);
	Request->SetURL(ScriptURL);
	Request->SetVerb(RequestType);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
}

bool UWebRequestBase::CallWebScript(const FString& ScriptURL, TSharedPtr<FJsonObject>& JsonRequest, EWebRequestType RequestType)
{
	if(!Http) Http = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = Http->CreateRequest();

	FString ResultURL = "http://";
	if(bUsingSSL) ResultURL = "https://";

	ResultURL += ScriptURL;

	RequestType == EWebRequestType::Post ? InitRequest(HttpRequest, "Post", ResultURL) : InitRequest(HttpRequest, "Get", ResultURL);

	TSharedRef<TJsonWriter<>>JsonWriter = TJsonWriterFactory<>::Create(&JsonStream);
	FJsonSerializer::Serialize(JsonRequest.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(JsonStream);

	UE_LOG(LogWebRequest, Log, TEXT("Request json data to:  %s"), *ResultURL);

	HttpRequest->ProcessRequest();
	
	return true;
}



