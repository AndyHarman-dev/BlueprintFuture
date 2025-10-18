// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibraries/BlueprintFutureFunctionLibrary.h"
#define LOG_CATEGORY_NAME LogFutureFunctionLib
#include "EasyLog.h"
#include "BlueprintFutureTypes/BlueprintPromiseHandle.h"

UBlueprintFutureHandle* UBlueprintFutureFunctionLibrary::ThenBunch(const TArray<UBlueprintFutureHandle*>& Futures)
{
	if (Futures.IsEmpty())
	{
		LOG_WARNING("Empty futures were given!");
		return UBlueprintPromiseHandle::MakeFulfilledPromise()->GetFuture();
	}

	TSharedPtr<int32> CounterPtr = MakeShared<int32>(0);
	auto Promise = UBlueprintPromiseHandle::MakePromise();
	auto FinalFuture = Promise->GetFuture();

	for (UBlueprintFutureHandle* Future : Futures)
	{
		Future->Then([CounterPtr, Promise, Futures](UBlueprintFutureHandle* Future)
		{
			(*CounterPtr)++;
			if (*CounterPtr == Futures.Num())
			{
				FBunchOfResolvedFutures BunchOfFutures{ Futures };
				Promise->SetValue(FSupportedTypesVariant{.AnyStruct = FInstancedStruct::Make(BunchOfFutures)});
			}
		});
	}

	return FinalFuture;
}
