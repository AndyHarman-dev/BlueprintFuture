// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PodTypeFunctionLibrary.generated.h"

struct FSupportedPodType;
/**
 * 
 */
UCLASS()
class BLUEPRINTFUTUREMODULE_API UPodTypeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, meta=(DisplayName="Make Supported Pod Type (Bool)"), Category="BlueprintFutureModule|PodType")
	static FSupportedPodType MakeSupportedPodType_Bool(bool Value);

	UFUNCTION(BlueprintPure, meta=(DisplayName="Make Supported Pod Type (Int)"), Category="BlueprintFutureModule|PodType")
	static FSupportedPodType MakeSupportedPodType_Int(int32 Value);

	UFUNCTION(BlueprintPure, meta=(DisplayName="Make Supported Pod Type (Float)"), Category="BlueprintFutureModule|PodType")
	static FSupportedPodType MakeSupportedPodType_Float(float Value);

	UFUNCTION(BlueprintPure, meta=(DisplayName="Make Supported Pod Type (String)"), Category="BlueprintFutureModule|PodType")
	static FSupportedPodType MakeSupportedPodType_String(FString Value);

	UFUNCTION(BlueprintPure, meta=(DisplayName="Make Supported Pod Type (Text)"), Category="BlueprintFutureModule|PodType")
	static FSupportedPodType MakeSupportedPodType_Text(FText Value);
	
};
