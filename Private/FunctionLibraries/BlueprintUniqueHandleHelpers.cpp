// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibraries/BlueprintUniqueHandleHelpers.h"

#include "BlueprintFutureTypes/BlueprintFutureHandle.h"

FMovedBlueprintPromiseHandle UBlueprintUniqueHandleHelpers::MoveHandle_Promise(UBlueprintPromiseHandle*& OldHandle)
{
	FMovedBlueprintPromiseHandle NewHandle(OldHandle);
	OldHandle = nullptr;
	return NewHandle;
}

UBlueprintPromiseHandle* UBlueprintUniqueHandleHelpers::ResolveMovedHandle_Promise(
	FMovedBlueprintPromiseHandle& Handle)
{
	auto DuplicatedObject = Handle.Handle;
	Handle.Handle = nullptr;
	return DuplicatedObject;
}

FMovedBlueprintFutureHandle UBlueprintUniqueHandleHelpers::MoveHandle_Future(UBlueprintFutureHandle*& OldHandle)
{
	FMovedBlueprintFutureHandle MovedHandle(OldHandle);
	OldHandle = nullptr;
	return MovedHandle;
}

UBlueprintFutureHandle* UBlueprintUniqueHandleHelpers::ResolveMovedHandle_Future(FMovedBlueprintFutureHandle& Handle)
{
	auto DuplicatedObject = Handle.Handle;
	Handle.Handle = nullptr;
	return DuplicatedObject;
}
