// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintFutureHandle.h"
#include "SupportedTypes.h"
#include "AttorneyPatternModule/Public/Helpers.h"
#include "BlueprintPromiseHandle.generated.h"

/**
 * Represents a handle to a promise used in the Blueprint Future system. This structure
 * allows the creation of a promise and the subsequent retrieval of a future handle
 * associated with it.
 */
UCLASS(BlueprintInternalUseOnly)
class BLUEPRINTFUTUREMODULE_API UBlueprintPromiseHandle : public UObject
{
	GENERATED_BODY()

	BEFRIEND_ATTORNEY(BlueprintPromiseHandle)
public:

	UFUNCTION(BlueprintCallable, Category="BlueprintFutureModule|Promise")
	static UBlueprintPromiseHandle* MakePromise()
	{
		auto Object =  NewObject<UBlueprintPromiseHandle>();
		Object->InternalPromise = MakeShareable(new TPromise<FSupportedTypesVariant>());
		Object->bFutureRetrieved = false;
		return Object;
	}
	
	UFUNCTION(BlueprintCallable, Category="BlueprintFutureModule|Promise")
	static UBlueprintPromiseHandle* MakeFulfilledPromise()
	{
		const auto Promise = MakePromise();
		Promise->SetEmptyValue();
		return Promise;
	}
	
	UFUNCTION(BlueprintCallable, Category="BlueprintFutureModule|Promise")
	void SetValue(const FSupportedTypesVariant& Value);

	UFUNCTION(BlueprintCallable, Category="BlueprintFutureModule|Promise")
	void SetEmptyValue();

	UFUNCTION(BlueprintCallable, Category="BlueprintFutureModule|Promise")
	UBlueprintFutureHandle* GetFuture();

	UFUNCTION(BlueprintPure, Category="BlueprintFutureModule|Promise")
	bool IsValid() const noexcept { return InternalPromise.IsValid(); }

	UFUNCTION(BlueprintCallable, Category="BlueprintFutureModule|Promise")
	void Invalidate() { InternalPromise.Reset(); }

protected:
	std::atomic<bool> bFutureRetrieved;
	TSharedPtr<TPromise<FSupportedTypesVariant>> InternalPromise;

	bool IsFutureRetrieved() const noexcept { return bFutureRetrieved.load(std::memory_order_relaxed); }
};

USTRUCT(BlueprintType, Blueprintable)
struct BLUEPRINTFUTUREMODULE_API FMovedBlueprintPromiseHandle
{
	GENERATED_BODY()

	UPROPERTY()
	UBlueprintPromiseHandle* Handle;
};
