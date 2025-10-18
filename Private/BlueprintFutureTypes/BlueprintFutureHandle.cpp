// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFutureTypes/BlueprintFutureHandle.h"
#include "BlueprintFutureTypes/BlueprintPromiseHandle.h"

#define LOG_CATEGORY_NAME LogFutureHandle
#include "EasyLog.h"

void UBlueprintFutureHandle::Wait() const
{
	while (!IsReady())
	{
		WaitFor(FTimespan::FromMilliseconds(10.0f));
	}
}

bool UBlueprintFutureHandle::WaitFor(FTimespan Timespan) const
{
	if (!InternalFuture)
	{
		LOG_ERROR("Future isn't set!");
		return false;
	}

	while (!InternalFuture->IsReady())
	{
		[[maybe_unused]] const bool unused = InternalFuture->WaitFor(Timespan);
	}

	return true;
}

void UBlueprintFutureHandle::BP_Then(const FFutureContinuation& Callback)
{
	Then([Callback](UBlueprintFutureHandle* Future)
	{
		Callback.ExecuteIfBound(Future);
	});
}

UBlueprintFutureHandle* UBlueprintFutureHandle::BP_Next_ReturnFuture(const FNextFutureContinuation_Future& Callback)
{
	return Next([Callback](const FSupportedTypesVariant& Value)
	{
		return Callback.Execute(Value);
	});
}

UBlueprintFutureHandle* UBlueprintFutureHandle::BP_Next_ReturnValue(const FNextFutureContinuation_Value& Callback)
{
	return Next([Callback](const FSupportedTypesVariant& Value)
	{
		return Callback.Execute(Value);
	});
}

void UBlueprintFutureHandle::Then(TFunction<void(UBlueprintFutureHandle*)> Continuation)
{
	InternalFuture->Then([ContinuationCapture = MoveTemp(Continuation)](TFuture<FSupportedTypesVariant>&& CompletedFuture) mutable
	{
		ContinuationCapture(MakeFuture(MoveTemp(CompletedFuture)));
	});
}

UBlueprintFutureHandle* UBlueprintFutureHandle::Next(
	const TFunction<UBlueprintFutureHandle*(FSupportedTypesVariant)>& Continuation)
{
	UBlueprintPromiseHandle* Promise = UBlueprintPromiseHandle::MakePromise();
	auto ResultFuture = Promise->GetFuture();

	auto Future = InternalFuture->Next(Continuation);
	Future.Then([Promise](TFuture<UBlueprintFutureHandle*>&& Future) mutable
	{
		Promise->SetValue(Future.Get()->GetValue());
	});
	
	return ResultFuture;
}

UBlueprintFutureHandle* UBlueprintFutureHandle::Next(
	const TFunction<FSupportedTypesVariant(FSupportedTypesVariant)>& Continuation)
{
	return MakeFuture(InternalFuture->Next(Continuation));
}

const FSupportedTypesVariant& UBlueprintFutureHandle::GetValue() const noexcept
{
	return InternalFuture->Get();
}

FSupportedTypesVariant& UBlueprintFutureHandle::GetMutableValue() noexcept
{
	return InternalFuture->GetMutable();
}