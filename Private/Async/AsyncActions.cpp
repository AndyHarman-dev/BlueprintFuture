// Fill out your copyright notice in the Description page of Project Settings.


#include "Async/AsyncActions.h"

#include "BlueprintFutureTypes/BlueprintFutureHandle.h"
#include "FunctionLibraries/BlueprintFutureFunctionLibrary.h"
#include "SafeAccessors/Public/MacroHelpers.h"

void UAsyncAction_Failable::Activate()
{
	Super::Activate();
	if (IsFailed())
	{
		Failed.Broadcast();
		return;
	}
}

UAsyncAction_ThenFuture* UAsyncAction_ThenFuture::Then(UBlueprintFutureHandle* Future)
{
	SAFE_ACCESS_RETURN(Future, MakeFailed<ThisClass>());
	SAFE_ACCESS_RETURN(Future->GetWorld(), MakeFailed<ThisClass>());
	
	const auto NewAction = NewObject<ThisClass>();
	NewAction->RegisterWithGameInstance(Future->GetWorld());
	NewAction->Future = Future;
	return NewAction;
}

void UAsyncAction_ThenFuture::Activate()
{
	Super::Activate();

	Future->Then([this](UBlueprintFutureHandle* Future)
	{
		Continuation.Broadcast(Future);
		SetReadyToDestroy();
	});
}

UAsyncAction_ThenBunchFuture* UAsyncAction_ThenBunchFuture::ThenBunch(const TArray<UBlueprintFutureHandle*>& Bunch)
{
	if (Bunch.IsEmpty())
	{
		return MakeFailed<ThisClass>();	
	}
	
	SAFE_ACCESS_RETURN(Bunch[0]->GetWorld(), MakeFailed<ThisClass>());
	
	const auto NewAction = NewObject<ThisClass>();
	NewAction->RegisterWithGameInstance(Bunch[0]->GetWorld());
	NewAction->Bunch = Bunch;
	return NewAction;
}

void UAsyncAction_ThenBunchFuture::Activate()
{
	Super::Activate();
	auto Gathered = UBlueprintFutureFunctionLibrary::ThenBunch(Bunch);
	Gathered->Then([this](UBlueprintFutureHandle* Future)
	{
		Continuation.Broadcast(Bunch);
		SetReadyToDestroy();
	});
}
