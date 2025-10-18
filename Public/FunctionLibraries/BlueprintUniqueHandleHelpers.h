// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintFutureTypes/BlueprintPromiseHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintUniqueHandleHelpers.generated.h"

class UBlueprintFutureHandle;
struct FMovedBlueprintFutureHandle;

/**
 * A static function library that provides utilities for manipulating and managing unique promise handles.
 * Includes functionality for moving and resolving promise handles in a Blueprint-friendly manner.
 */
UCLASS()
class BLUEPRINTFUTUREMODULE_API UBlueprintUniqueHandleHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	* This functions invalidates OldHandle in order to transfer ownership of the handle.
	* @param OldHandle - handle that you are going to move to the new copy
	* @return A copy of the @OldHandle.
	*/
	UFUNCTION(BlueprintCallable, meta=(DisplayName="MoveHandle"), Category="BlueprintFuture|Promise")
	static FMovedBlueprintPromiseHandle MoveHandle_Promise(UBlueprintPromiseHandle*& OldHandle);

	/**
	 * This function will resolve a moved UBlueprintPromiseHandle and invalidate the moved handle.
	 * @param Handle - handle to resolve
	 * @return Blueprint promise handle
	 */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="MoveHandle"), Category="BlueprintFuture|Promise")
	static UBlueprintPromiseHandle* ResolveMovedHandle_Promise(UPARAM(ref) FMovedBlueprintPromiseHandle& Handle);
	
	/**
	* This functions invalidates OldHandle in order to transfer ownership of the handle.
	* @param OldHandle - handle that you are going to move to the new copy
	* @return A copy of the @OldHandle.
	*/
	UFUNCTION(BlueprintCallable, meta=(DisplayName="MoveHandle"), Category="BlueprintFuture|Future")
	static FMovedBlueprintFutureHandle MoveHandle_Future(UBlueprintFutureHandle*& OldHandle);

	/**
	 * This function will resolve a moved UBlueprintPromiseHandle and invalidate the moved handle.
	 * @param Handle - handle to resolve
	 * @return Blueprint promise handle
	 */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="MoveHandle"), Category="BlueprintFuture|Future")
	static UBlueprintFutureHandle* ResolveMovedHandle_Future(UPARAM(ref) FMovedBlueprintFutureHandle& Handle);
	
};
