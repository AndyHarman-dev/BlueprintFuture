// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFutureTypes/BlueprintPromiseHandle.h"
#include "ValidateMacro.h"

DEFINE_LOG_CATEGORY_STATIC(LogPromiseHandle, Log, All);

void UBlueprintPromiseHandle::SetValue(const FSupportedTypesVariant& Value)
{
	if (InternalPromise)
	{
		InternalPromise->SetValue(Value);
	}
}

void UBlueprintPromiseHandle::SetEmptyValue()
{
	if (InternalPromise)
	{
		InternalPromise->SetValue({});
	}
}

UBlueprintFutureHandle* UBlueprintPromiseHandle::GetFuture()
{
	if (bFutureRetrieved.load(std::memory_order_relaxed))
	{
		UE_LOG(LogTemp, Error, TEXT("Future already retrieved"));
		return nullptr;
	}

	bFutureRetrieved.store(true, std::memory_order_relaxed);
	return UBlueprintFutureHandle::MakeFuture(InternalPromise->GetFuture());
}