// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

#define GET_NAME(n) #n

FString UMyBlueprintFunctionLibrary::ReceiveSomeStruct(UProperty* AnyStruct)
{

	FString a = FString::Printf(TEXT("None Struct"));
	if (AnyStruct)
	{
		a = FString::Printf(TEXT("in Struct"));
		return a;
		//return GET_NAME(AnyStruct);
	}

	return a;
}

//FString UMyBlueprintFunctionLibrary::GetVariableName(TWeakObjectPtr<UObject> Node)
//{
//	
//	return "";
//}