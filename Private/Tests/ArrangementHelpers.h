#pragma once
#include "CoreMinimal.h"
#include "BlueprintFutureTypes/BlueprintPromiseHandle.h"

namespace ArrangementHelpers
{
	inline TTuple<UBlueprintPromiseHandle*, UBlueprintFutureHandle*> CreatePromiseAndGetItsFuture()
	{
		UBlueprintPromiseHandle* Promise = UBlueprintPromiseHandle::MakePromise();
		UBlueprintFutureHandle* Future = Promise->GetFuture();
		return MakeTuple(Promise, Future);
	}
}
