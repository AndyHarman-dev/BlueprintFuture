// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintFutureFunctionLibrary.generated.h"

class UBlueprintFutureHandle;

USTRUCT()
struct FBunchOfResolvedFutures
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<UBlueprintFutureHandle*> Futures;
};

/**
 * 
 */
UCLASS()
class BLUEPRINTFUTUREMODULE_API UBlueprintFutureFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category="BlueprintFuture")
	static UBlueprintFutureHandle* ThenBunch(const TArray<UBlueprintFutureHandle*>& Futures);
};
