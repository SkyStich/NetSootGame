// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShotgunObject.h"

UBaseShotgunObject::UBaseShotgunObject()
{

}

void UBaseShotgunObject::DropLineTrace()
{
	for(int32 i = 0; i < 7; i++)
	{
		Super::DropLineTrace();
	}
}