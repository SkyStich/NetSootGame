// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkWithTime.h"

FString UWorkWithTime::GetTimeByStandard(int32 Min, int32 Sec)
{
	/** Format text */
	FString Minutes;
	FString Seconds;
	Min < 10 ? Minutes = "0" + FString::FromInt(Min) : Minutes = FString::FromInt(Min);
	Sec < 10 ? Seconds = "0" + FString::FromInt(Sec) : Seconds = FString::FromInt(Sec);

	return FString(Minutes + ":" + Seconds);
}

FString UWorkWithTime::ConvertSecondOnTimeStandard(int32 Sec)
{
	/** Get minutes and seconds from total seconds */
	int32 const Minutes = Sec / 60;
	int32 const Seconds = Sec % (Minutes * 60);

	FString StringMin;
	FString StringSec;
	
	Minutes < 10 ? StringMin = "0" + FString::FromInt(Minutes) : StringMin = FString::FromInt(Minutes);
	Seconds < 10 ? StringSec = "0" + FString::FromInt(Seconds) : StringSec = FString::FromInt(Seconds);

	return FString(StringMin + ":" + StringSec);
}

void UWorkWithTime::ConvertTotalSecondInStandardTime(int32 const Sec, int32 & ReturnSeconds, int32 & ReturnMinutes)
{
	ReturnMinutes = Sec / 60;
	ReturnSeconds = Sec % (ReturnMinutes * 60);
}