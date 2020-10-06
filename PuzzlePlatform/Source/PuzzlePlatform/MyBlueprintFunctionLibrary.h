// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, CustomThunk, meta = (DisplayName = "GetVariableName", CompactNodeTitle = "Getname"), Category = "MyETC")
		static FString GetVariableName(float A, float B = 1.f);

	static FString Generic_GetVariableName(float A, float B);

	DECLARE_FUNCTION(execGetVariableName)
	{
		P_GET_PROPERTY(UFloatProperty, A);
		P_GET_PROPERTY(UFloatProperty, B);

		P_FINISH;

		if (B == 0.f)
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("Divide by zero detected: %f / 0\n%s"), A, *Stack.GetStackTrace()), ELogVerbosity::Warning);
			*(float*)RESULT_PARAM = 0;
			return;
		}

		*(FString*)RESULT_PARAM = Generic_GetVariableName(A, B);
	}


	//UFUNCTION(BlueprintPure, CustomThunk, meta = (DisplayName = "GetVariableName1", CompactNodeTitle = "Getname"), Category = "MyETC")
	//	static FString GetVariableName1(UProperty* A, float B = 1.f);

	//static FString Generic_GetVariableName1(UProperty* A, float B);

	//DECLARE_FUNCTION(execGetVariableName1)
	//{

	//	// Steps into the stack, walking to the next property in it
	//	/*Stack.Step(Stack.Object, NULL);

	//	UProperty* StructProperty = Cast<UProperty>(Stack.MostRecentProperty);

	//	void* StructPtr = Stack.MostRecentPropertyAddress;*/

	//	//P_GET_PROPERTY(UFloatProperty, A);
	//	P_GET_PROPERTY(UFloatProperty, B);

	//	P_FINISH;

	//	if (B == 0.f)
	//	{
	//		//FFrame::KismetExecutionMessage(*FString::Printf(TEXT("Divide by zero detected: %f / 0\n%s"), A, *Stack.GetStackTrace()), ELogVerbosity::Warning);
	//		*(float*)RESULT_PARAM = 0;
	//		return;
	//	}

	//	//*(FString*)RESULT_PARAM = Generic_GetVariableName1(StructProperty, B);
	//}

};