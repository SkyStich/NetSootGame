// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSingletonClass.h"
#include "Engine/Engine.h"

UGameSingletonClass::UGameSingletonClass()
{
    
}

bool UGameSingletonClass::Tick(float DeltaTime)
{
    return true;
}

/** Get Game Singleton class */
UGameSingletonClass& UGameSingletonClass::Get()
{
    UGameSingletonClass* Singleton = Cast<UGameSingletonClass>(GEngine->GameSingleton);

    if(Singleton)
    {
        return *Singleton;
    }
    else
    {
        return *NewObject<UGameSingletonClass>(UGameSingletonClass::StaticClass()); 
    }
}