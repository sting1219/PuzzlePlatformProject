// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

#define GET_NAME(n) #n

FString UMyBlueprintFunctionLibrary::GetVariableName(float A, float B)
{
	check(0);
	return FString::Printf(TEXT("GetVariableName"));
}

FString UMyBlueprintFunctionLibrary::Generic_GetVariableName(float A, float B)
{

	return FString::Printf(TEXT("Generic_GetVariableName : %f"), A + B);
}

//FString UMyBlueprintFunctionLibrary::GetVariableName1(UProperty* A, float B)
//{
//	check(0);
//	return FString::Printf(TEXT("GetVariableName1"));
//}
//
//FString UMyBlueprintFunctionLibrary::Generic_GetVariableName1(UProperty* A, float B)
//{
//
//	return FString::Printf(TEXT("Generic_GetVariableName1 : %f"), B);
//}

