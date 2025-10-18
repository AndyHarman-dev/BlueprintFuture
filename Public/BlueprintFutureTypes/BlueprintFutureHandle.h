// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SupportedTypes.h"
#include "BlueprintFutureHandle.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FFutureContinuation, UBlueprintFutureHandle*, Future);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(UBlueprintFutureHandle*, FNextFutureContinuation_Future, const FSupportedTypesVariant&, Value);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(FSupportedTypesVariant, FNextFutureContinuation_Value, const FSupportedTypesVariant&, Value);

/**
 * Represents a handle to a future in the BlueprintFuture module, enabling asynchronous operation
 * handling and chaining.
 */
UCLASS(BlueprintInternalUseOnly)
class BLUEPRINTFUTUREMODULE_API UBlueprintFutureHandle : public UObject
{
	GENERATED_BODY()

public:

	static UBlueprintFutureHandle* MakeFuture(TFuture<FSupportedTypesVariant>&& InternalFuture)
	{
		auto Object = NewObject<UBlueprintFutureHandle>();
		Object->InternalFuture = MakeShareable(new TFuture<FSupportedTypesVariant>(MoveTemp(InternalFuture)));
		return Object;
	}
	
	UFUNCTION(BlueprintCallable, Category="BlueprintFutureModule|Future")
	void Invalidate() { InternalFuture.Reset(); }

	// Waits untill future completion
	UFUNCTION(BlueprintCallable, Category="BlueprintFutureModule|Future")
	void Wait() const;

	UFUNCTION(BlueprintCallable, Category="BlueprintFutureModule|Future")
	bool WaitFor(FTimespan Timespan) const;

	UFUNCTION(BlueprintPure, Category="BlueprintFutureModule|Future")
	bool IsReady() const noexcept { return InternalFuture->IsReady(); }

	UFUNCTION(BlueprintPure, Category="BlueprintFutureModule|Future")
	bool IsValid() const noexcept { return InternalFuture && InternalFuture->IsValid(); }

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Then"), Category="BlueprintFutureModule|Future")
	void BP_Then(const FFutureContinuation& Callback);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Next (Callback Returns Future)"), Category="BlueprintFutureModule|Future")
	UBlueprintFutureHandle* BP_Next_ReturnFuture(const FNextFutureContinuation_Future& Callback);

	UFUNCTION(BlueprintCallable, meta=(DisplayName="Next (Callback Returns Value)"), Category="BlueprintFutureModule|Future")
	UBlueprintFutureHandle* BP_Next_ReturnValue(const FNextFutureContinuation_Value& Callback);

	void Then(TFunction<void(UBlueprintFutureHandle*)> Continuation);
	UBlueprintFutureHandle* Next(const TFunction<UBlueprintFutureHandle*(FSupportedTypesVariant)>& Continuation);
	UBlueprintFutureHandle* Next(const TFunction<FSupportedTypesVariant(FSupportedTypesVariant)>& Continuation);
	
	UFUNCTION(BlueprintPure, Category="BlueprintFutureModule|Future")
	const FSupportedTypesVariant& GetValue() const noexcept;

	UFUNCTION(BlueprintPure, Category="BlueprintFutureModule|Future")
	FSupportedTypesVariant& GetMutableValue() noexcept;
	
private:
	TSharedPtr<TFuture<FSupportedTypesVariant>> InternalFuture = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct BLUEPRINTFUTUREMODULE_API FMovedBlueprintFutureHandle
{
	GENERATED_BODY()

	UPROPERTY()
	UBlueprintFutureHandle* Handle;
};
