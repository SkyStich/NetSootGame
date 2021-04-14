// Fill out your copyright notice in the Description page of Project Settings.


#include "WebRequest.h"

void UWebRequest::CallScript(const FString& ScriptURL)
{
	TSharedPtr<FJsonObject> Json = CreateJsonRequest();

	for(const auto& ByArray : KeyNumbers)
	{
		Json->SetNumberField(ByArray.Key, ByArray.Value);
	}

	for(const auto& ByArray : KeyStrings)
	{
		Json->SetStringField(ByArray.Key, ByArray.Value);
	}
	CallWebScript(ScriptURL, Json);
}

void UWebRequest::CallJsonResponse(const TSharedPtr<FJsonObject>& JsonResponse)
{
	FString ResultString = "";
	double ResultNumber = 0.f;

	if(!JsonResponse->HasField("ResultText")) return;

	ResultNumber = JsonResponse->GetNumberField("ResultNumber");
	ResultString = JsonResponse->GetStringField("ResultText");

	float ResultFloat = (float) ResultNumber;

	UE_LOG(LogTemp, Warning, TEXT("Web responce: string = %s, Number = %f"), *ResultString, ResultFloat);

	OnResultTest.Broadcast(ResultString, ResultFloat);
}

