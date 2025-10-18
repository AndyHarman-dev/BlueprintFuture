// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "AsyncActions.generated.h"

class UBlueprintFutureHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAsyncActionFailed);

template<typename TAnyType>
concept DerivedFromFailable = std::is_base_of_v<class UAsyncAction_Failable, TAnyType>;

UCLASS()
class UAsyncAction_Failable : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAsyncActionFailed Failed;

	template<DerivedFromFailable Failable>
	static Failable* MakeFailed()
	{
		auto NewAction = NewObject<Failable>();
		NewAction->bFailed = true;
		return NewAction;
	}
	bool IsFailed() const noexcept { return bFailed; }

protected:
	virtual void Activate() override;
	
private:
	bool bFailed = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFutureContinued, UBlueprintFutureHandle*, Future);

/**
 * 
 */
UCLASS()
class BLUEPRINTFUTUREMODULE_API UAsyncAction_ThenFuture : public UAsyncAction_Failable
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly=true), Category="BlueprintFuture|Async")
	static UAsyncAction_ThenFuture* Then(UBlueprintFutureHandle* Future);
	
	UPROPERTY(BlueprintAssignable)
	FOnFutureContinued Continuation;

private:
	UPROPERTY()
	TObjectPtr<UBlueprintFutureHandle> Future;

	virtual void Activate() override;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThenBunchFutureContinuaion, const TArray<UBlueprintFutureHandle*>&, Handles);

UCLASS()
class BLUEPRINTFUTUREMODULE_API UAsyncAction_ThenBunchFuture : public UAsyncAction_Failable
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly=true), Category="BlueprintFuture|Async")
	static UAsyncAction_ThenBunchFuture* ThenBunch(const TArray<UBlueprintFutureHandle*>& Bunch);
	
	UPROPERTY(BlueprintAssignable)
	FOnThenBunchFutureContinuaion Continuation;

private:
	UPROPERTY()
	TArray<UBlueprintFutureHandle*> Bunch;

	virtual void Activate() override;
};

