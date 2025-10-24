
#if WITH_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "CoreMinimal.h"
#include "ArrangementHelpers.h"
#include "Misc/AutomationTest.h"
#include "Engine/Engine.h"
#include "BlueprintFutureTypes/BlueprintFutureHandle.h"
#include "BlueprintFutureTypes/BlueprintPromiseHandle.h"
#include "BlueprintFutureTypes/SupportedTypes.h"
#include "Scoped/LogErrorSuppression.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintPromise_GetFuture_OnlyOnce, "BlueprintFutureModule.Promise.GetFuture.OnlyOnce", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintPromise_GetFuture_OnlyOnce::RunTest(const FString& Parameters)
{
    FScopedLogErrorSuppression ErrorSuppression(this);
    
    // Arrange
    UBlueprintPromiseHandle* PromiseHandle = UBlueprintPromiseHandle::MakePromise();
    
    // Act
    UBlueprintFutureHandle* FirstFuture = PromiseHandle->GetFuture();
    UBlueprintFutureHandle* SecondFuture = PromiseHandle->GetFuture();
    
    // Assert
    TestTrue("First future retrieval should succeed", FirstFuture && FirstFuture->IsValid());
    TestFalse("Second future retrieval should fail", SecondFuture && SecondFuture->IsValid());

    PromiseHandle->SetEmptyValue();
    return true;
}

// Test Promise Resolution
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintPromise_Resolve_Boolean, "BlueprintFutureModule.Promise.Resolve.Boolean", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintPromise_Resolve_Boolean::RunTest(const FString& Parameters)
{
    // Arrange
    auto[PromiseHandle, FutureHandle] = ArrangementHelpers::CreatePromiseAndGetItsFuture();
    
    FSupportedTypesVariant TestValue;
    TestValue.PodType.Type = EPodType::Boolean;
    TestValue.PodType.Boolean = true;
    
    // Act
    PromiseHandle->SetValue(TestValue);
    FutureHandle->Wait();
    const FSupportedTypesVariant& RetrievedValue = FutureHandle->GetValue();
    
    // Assert
    TestEqual("Retrieved value type should match", RetrievedValue.PodType.Type, EPodType::Boolean);
    TestEqual("Retrieved boolean value should match", RetrievedValue.PodType.Boolean, true);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintPromise_Resolve_Integer, "BlueprintFutureModule.Promise.Resolve.Integer", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintPromise_Resolve_Integer::RunTest(const FString& Parameters)
{
    // Arrange
    auto[PromiseHandle, FutureHandle] = ArrangementHelpers::CreatePromiseAndGetItsFuture();
    
    FSupportedTypesVariant TestValue;
    TestValue.PodType.Type = EPodType::Int;
    TestValue.PodType.Int = 42;
    
    // Act
    PromiseHandle->SetValue(TestValue);
    FutureHandle->Wait();
    const FSupportedTypesVariant& RetrievedValue = FutureHandle->GetValue();
    
    // Assert
    TestEqual("Retrieved value type should match", RetrievedValue.PodType.Type, EPodType::Int);
    TestEqual("Retrieved integer value should match", RetrievedValue.PodType.Int, 42);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintPromise_Resolve_String, "BlueprintFutureModule.Promise.Resolve.String", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintPromise_Resolve_String::RunTest(const FString& Parameters)
{
    // Arrange
    auto[PromiseHandle, FutureHandle] = ArrangementHelpers::CreatePromiseAndGetItsFuture();
    
    FSupportedTypesVariant TestValue;
    TestValue.PodType.Type = EPodType::String;
    TestValue.PodType.String = TEXT("Hello World");
    
    // Act
    PromiseHandle->SetValue(TestValue);
    FutureHandle->Wait();
    const FSupportedTypesVariant& RetrievedValue = FutureHandle->GetValue();
    
    // Assert
    TestEqual("Retrieved value type should match", RetrievedValue.PodType.Type, EPodType::String);
    TestEqual("Retrieved string value should match", RetrievedValue.PodType.String, TEXT("Hello World"));
    
    return true;
}

// Test Future Continuation with Next
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintFuture_Next_SimpleTransformation, "BlueprintFutureModule.Future.Next.SimpleTransformation", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintFuture_Next_SimpleTransformation::RunTest(const FString& Parameters)
{
    // Arrange
    auto[PromiseHandle, OriginalFuture] = ArrangementHelpers::CreatePromiseAndGetItsFuture();
    
    // Act
    UBlueprintFutureHandle* NextFuture = OriginalFuture->Next(
        [](const FSupportedTypesVariant& Input) -> FSupportedTypesVariant {
        FSupportedTypesVariant Output;
        Output.PodType.Type = EPodType::Int;
        Output.PodType.Int = Input.PodType.Int * 2;
        return Output;
    });
    
    FSupportedTypesVariant InputValue;
    InputValue.PodType.Type = EPodType::Int;
    InputValue.PodType.Int = 21;
    PromiseHandle->SetValue(InputValue);
    
    NextFuture->Wait();
    const FSupportedTypesVariant& Result = NextFuture->GetValue();
    
    // Assert
    TestTrue("Next future should be valid", NextFuture->IsValid());
    TestEqual("Result type should be integer", Result.PodType.Type, EPodType::Int);
    TestEqual("Result value should be doubled", Result.PodType.Int, 42);
    
    return true;
}

// Test Async Callbacks with Then
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintFuture_Then_AsyncCallback, "BlueprintFutureModule.Future.Then.AsyncCallback", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintFuture_Then_AsyncCallback::RunTest(const FString& Parameters)
{
    // Arrange
    auto[PromiseHandle, FutureHandle] = ArrangementHelpers::CreatePromiseAndGetItsFuture();
    
    bool bCallbackTriggered = false;
    
    // Act
    FutureHandle->Then(
    [&bCallbackTriggered](auto&& Handle) {
        bCallbackTriggered = true;
    });
    
    FSupportedTypesVariant TestValue;
    TestValue.PodType.Type = EPodType::Boolean;
    TestValue.PodType.Boolean = true;
    PromiseHandle->SetValue(TestValue);
    
    // Assert
    TestTrue("Callback should have been triggered", bCallbackTriggered);
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintFuture_GetValue_InvalidHandle, "BlueprintFutureModule.Future.GetValue.InvalidHandle", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintFuture_GetValue_InvalidHandle::RunTest(const FString& Parameters)
{
    // Arrange
    UBlueprintFutureHandle* InvalidHandle = nullptr; // Default constructor creates invalid handle
    
    // Act & Assert
    TestFalse("Future must be invalid", InvalidHandle);
    
    return true;
}

// Test Handle Equality and Comparison
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintUniqueHandle_Equality, "BlueprintFutureModule.UniqueHandle.Equality", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintUniqueHandle_Equality::RunTest(const FString& Parameters)
{
    // Arrange
    UBlueprintPromiseHandle* Handle1 = UBlueprintPromiseHandle::MakePromise();
    Handle1->SetEmptyValue();
    UBlueprintPromiseHandle* Handle2 = UBlueprintPromiseHandle::MakePromise();
    Handle2->SetEmptyValue();
    UBlueprintPromiseHandle* Handle1Copy = Handle1;
    
    // Act & Assert
    TestTrue("Handle should equal itself", Handle1 == Handle1);
    TestTrue("Handle should equal its copy", Handle1 == Handle1Copy);
    TestFalse("Different handles should not be equal", Handle1 == Handle2);
    
    return true;
}

// Test Handle Invalidation
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintUniqueHandle_Invalidation, "BlueprintFutureModule.UniqueHandle.Invalidation", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintUniqueHandle_Invalidation::RunTest(const FString& Parameters)
{
    // Arrange
    UBlueprintPromiseHandle* Handle = UBlueprintPromiseHandle::MakePromise();
    Handle->SetEmptyValue();
    
    // Act
    TestTrue("Handle should be valid initially", Handle->IsValid());
    Handle->Invalidate();
    
    // Assert
    TestFalse("Handle should be invalid after invalidation", Handle->IsValid());
    
    return true;
}

// Test Supported Types Variant
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_SupportedTypesVariant_AllPodTypes, "BlueprintFutureModule.SupportedTypes.AllPodTypes", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_SupportedTypesVariant_AllPodTypes::RunTest(const FString& Parameters)
{
    // Test Boolean
    {
        FSupportedTypesVariant Variant;
        Variant.PodType.Type = EPodType::Boolean;
        Variant.PodType.Boolean = true;
        TestEqual("Boolean type should be set correctly", Variant.PodType.Type, EPodType::Boolean);
        TestEqual("Boolean value should be set correctly", Variant.PodType.Boolean, true);
    }
    
    // Test Float
    {
        FSupportedTypesVariant Variant;
        Variant.PodType.Type = EPodType::Float;
        Variant.PodType.Float = 3.14f;
        TestEqual("Float type should be set correctly", Variant.PodType.Type, EPodType::Float);
        TestEqual("Float value should be set correctly", Variant.PodType.Float, 3.14f, 0.001f);
    }
    
    // Test Text
    {
        FSupportedTypesVariant Variant;
        Variant.PodType.Type = EPodType::Text;
        Variant.PodType.Text = FText::FromString(TEXT("Test Text"));
        TestEqual("Text type should be set correctly", Variant.PodType.Type, EPodType::Text);
        TestEqual("Text value should be set correctly", Variant.PodType.Text.ToString(), TEXT("Test Text"));
    }
    
    return true;
}

#endif
