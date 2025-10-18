// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibraries/PodTypeFunctionLibrary.h"

#include "BlueprintFutureTypes/SupportedTypes.h"

FSupportedPodType UPodTypeFunctionLibrary::MakeSupportedPodType_Bool(bool Value)
{
	return FSupportedPodType::FromBool(Value);
}

FSupportedPodType UPodTypeFunctionLibrary::MakeSupportedPodType_Int(int32 Value)
{
	return FSupportedPodType::FromInt(Value);
}

FSupportedPodType UPodTypeFunctionLibrary::MakeSupportedPodType_Float(float Value)
{
	return FSupportedPodType::FromFloat(Value);
}

FSupportedPodType UPodTypeFunctionLibrary::MakeSupportedPodType_String(FString Value)
{
	return FSupportedPodType::FromString(Value);
}

FSupportedPodType UPodTypeFunctionLibrary::MakeSupportedPodType_Text(FText Value)
{
	return FSupportedPodType::FromText(Value);
}
