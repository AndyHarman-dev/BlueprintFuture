// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"
#include "SupportedTypes.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EPodType : uint8
{
	Boolean,
	Int,
	Float,
	String,
	Text
};

/**
 * @brief A structure that represents the supported plain-old-data (POD) types.
 *
 * This structure is used to define and handle a range of fundamental data types
 * that are eligible for use in various systems requiring simple and lightweight data structures.
 */
USTRUCT(BlueprintType)
struct FSupportedPodType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BlueprintFuture")
	EPodType Type = EPodType::Boolean;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BlueprintFuture")
	bool Boolean = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BlueprintFuture")
	int32 Int = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BlueprintFuture")
	float Float = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BlueprintFuture")
	FString String = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BlueprintFuture")
	FText Text = FText::GetEmpty();

	static FSupportedPodType FromBool(bool Value)
	{
		FSupportedPodType Result;
		Result.Type = EPodType::Boolean;
		Result.Boolean = Value;
		return Result;
	}

	static FSupportedPodType FromInt(int32 Value)
	{
		FSupportedPodType Result;
		Result.Type = EPodType::Int;
		Result.Int = Value;
		return Result;
	}

	static FSupportedPodType FromFloat(float Value)
	{
		FSupportedPodType Result;
		Result.Type = EPodType::Float;
		Result.Float = Value;
		return Result;
	}

	static FSupportedPodType FromString(FString Value)
	{
		FSupportedPodType Result;
		Result.Type = EPodType::String;
		Result.String = Value;
		return Result;
	}

	static FSupportedPodType FromText(FText Value)
	{
		FSupportedPodType Result;
		Result.Type = EPodType::Text;
		Result.Text = Value;
		return Result;
	}
	
};

/**
 * @brief A structure that encapsulates various types of supported fields in a flexible variant form.
 *
 * This structure allows storing an object, an instanced struct, or a predefined set of plain-old-data (POD) types.
 * It is designed to be utilized in Blueprint systems for versatile and flexible data management.
 */
USTRUCT(BlueprintType)
struct FSupportedTypesVariant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BlueprintFuture")
	TObjectPtr<UObject> AnyObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BlueprintFuture")
	FInstancedStruct AnyStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BlueprintFuture")
	FSupportedPodType PodType;
};
