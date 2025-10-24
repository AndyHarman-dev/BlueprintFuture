#include "ArrangementHelpers.h"
#include "Misc/AutomationTest.h"

#if WITH_AUTOMATION_TESTS

// Test Multithreaded Promise Resolution and Future Handling
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintFuturePromise_MultithreadedExecution, "BlueprintFutureModule.Threading.MultithreadedExecution", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintFuturePromise_MultithreadedExecution::RunTest(const FString& Parameters)
{
    // Arrange
   
    // Create promise and future on main thread
    auto[PromiseHandle, FutureHandle] = ArrangementHelpers::CreatePromiseAndGetItsFuture();

    FCriticalSection CriticalSection;
    UBlueprintFutureHandle* CallbackFutureHandle;
    
    // Thread safety validation variables
    std::atomic<bool> bBackgroundThreadStarted{false};
    std::atomic<bool> bBackgroundThreadCompleted{false};
    std::atomic<bool> bCallbackExecuted{false};
    std::atomic<int32> BackgroundThreadId{0};
    std::atomic<int32> CallbackThreadId{0};
    
    const int32 MainThreadId = FPlatformTLS::GetCurrentThreadId();
    const int32 ExpectedValue = 12345;
    
    // Register the callback
    FutureHandle->Then([&bCallbackExecuted, &CallbackThreadId, &CallbackFutureHandle, &CriticalSection](UBlueprintFutureHandle* Handle) {

        FScopeLock Lock(&CriticalSection);
        CallbackFutureHandle = Handle;
        bCallbackExecuted = true;
        CallbackThreadId = FPlatformTLS::GetCurrentThreadId();
    });

    TestFalse("Future handle should've been invalidated by Then method", FutureHandle->IsValid());
    
    // Act - Resolve promise from background thread
    AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [&, PromiseHandle, ExpectedValue]() mutable {
        bBackgroundThreadStarted = true;
        BackgroundThreadId = FPlatformTLS::GetCurrentThreadId();
        
        // Simulate some work on background thread
        FPlatformProcess::Sleep(0.1f);
        
        // Create test value
        FSupportedTypesVariant TestValue;
        TestValue.PodType.Type = EPodType::Int;
        TestValue.PodType.Int = ExpectedValue;
        
        // Resolve promise from background thread
        PromiseHandle->SetValue(TestValue);
        bBackgroundThreadCompleted = true;
    });
    
    // Wait for background thread to start
    double StartTime = FPlatformTime::Seconds();
    while (!bBackgroundThreadStarted && (FPlatformTime::Seconds() - StartTime) < 5.0)
    {
        FPlatformProcess::Sleep(0.01f);
    }
    
    // Wait for callback to execute (it should be marshalled to game thread)
    StartTime = FPlatformTime::Seconds();
    while (!bCallbackExecuted && (FPlatformTime::Seconds() - StartTime) < 5.0)
    {
        // Tick the game thread to allow async tasks to execute
        FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
        FPlatformProcess::Sleep(0.01f);
    }
    
    // Get the resolved value
    FScopeLock Lock(&CriticalSection);
    CallbackFutureHandle->Wait();
    const FSupportedTypesVariant& RetrievedValue = CallbackFutureHandle->GetValue();
    
    // Assert
    TestTrue("Background thread should have started", bBackgroundThreadStarted);
    TestNotEqual("Background thread ID should be different from main thread", BackgroundThreadId.load(), MainThreadId);
    
    TestEqual("Retrieved value type should be integer", RetrievedValue.PodType.Type, EPodType::Int);
    TestEqual("Retrieved value should match expected value set from background thread", RetrievedValue.PodType.Int, ExpectedValue);
    TestTrue("Callback should have been executed", bCallbackExecuted);
    return true;
}

// Test Concurrent Future Chain Operations
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTest_BlueprintFuture_MultithreadedChaining, "BlueprintFutureModule.Threading.MultithreadedChaining", 
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FTest_BlueprintFuture_MultithreadedChaining::RunTest(const FString& Parameters)
{
    // Arrange
    auto[PromiseHandle, OriginalFuture] = ArrangementHelpers::CreatePromiseAndGetItsFuture();
    
    std::atomic<bool> bFirstContinuationExecuted{false};
    std::atomic<bool> bSecondContinuationExecuted{false};
    std::atomic<int32> FirstContinuationThreadId{0};
    std::atomic<int32> SecondContinuationThreadId{0};
    
    // Chain the continuations
    UBlueprintFutureHandle* FinalHandle = OriginalFuture->Next(
     [&bFirstContinuationExecuted, &FirstContinuationThreadId](const FSupportedTypesVariant& Input) -> FSupportedTypesVariant {
         bFirstContinuationExecuted = true;
         FirstContinuationThreadId = FPlatformTLS::GetCurrentThreadId();

         FSupportedTypesVariant Output;
         Output.PodType.Type = EPodType::Int;
         Output.PodType.Int = Input.PodType.Int * 2;
         return Output;
     })
    ->Next([&bSecondContinuationExecuted, &SecondContinuationThreadId](const FSupportedTypesVariant& Input) -> FSupportedTypesVariant {
        bSecondContinuationExecuted = true;
        SecondContinuationThreadId = FPlatformTLS::GetCurrentThreadId();
        
        FSupportedTypesVariant Output;
        Output.PodType.Type = EPodType::Int;
        Output.PodType.Int = Input.PodType.Int + 100;
        return Output;
    });
    
    const int32 InitialValue = 10;
    const int32 ExpectedFinalValue = (InitialValue * 2) + 100; // (10 * 2) + 100 = 120
    
    // Act - Resolve from background thread while continuations are set up
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [PromiseHandle, InitialValue]() mutable {
        // Add some delay to test race conditions
        FPlatformProcess::Sleep(0.05f);
        
        FSupportedTypesVariant InitialData;
        InitialData.PodType.Type = EPodType::Int;
        InitialData.PodType.Int = InitialValue;
        
        PromiseHandle->SetValue(InitialData);
    });
    
    // Wait for the final result
    FinalHandle->Wait();
    const FSupportedTypesVariant& FinalResult = FinalHandle->GetValue();
    
    // Allow time for all async operations to complete
    FPlatformProcess::Sleep(0.2f);
    FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
    
    // Assert
    TestTrue("First continuation should have executed", bFirstContinuationExecuted);
    TestTrue("Second continuation should have executed", bSecondContinuationExecuted);
    
    TestEqual("Final result type should be integer", FinalResult.PodType.Type, EPodType::Int);
    TestEqual("Final result should be correctly chained", FinalResult.PodType.Int, ExpectedFinalValue);
    
    TestFalse("Original future should not be valid", OriginalFuture->IsValid());
    TestTrue("Final future should be valid", FinalHandle->IsValid());
    
    return true;
}

#endif // WITH_AUTOMATION_TESTS