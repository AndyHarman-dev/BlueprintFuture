# BlueprintFutureModule

**BlueprintFutureModule** is an Unreal Engine C++ module that brings robust asynchronous programming support to Blueprints. It provides a set of utilities for working with promise/future objects, enabling you to compose, chain, and manage asynchronous operations visually within Blueprints or in C++ code. This module makes it simple for Unreal developers to implement modern async workflows in a Blueprint-friendly way, enhancing both code flexibility and maintainability.

***

## Features

- Native Unreal Engine plugin module supporting both C++ and Blueprint scripting
- **Future** and **Promise** handle types—enabling easy creation, fulfillment, and chaining of async operations
- Powerful Blueprint nodes to wait for, chain, and combine async operations ("futures")
- Support for **bunch** operations: wait on multiple futures and collect results
- Simple helpers for "moving" handles safely between contexts
- **Pod type function library** for generic data passing (Bool, Int, Float, String, Text)
- Full Blueprint integration for visual scripting of complex async logic

***

## Tech Stack

- **Language:** C++20
- **Game Engine:** Unreal Engine (UE4/UE5+)
- **Blueprints:** Full support via Blueprint Nodes/UFunctions
- **Logging:** Uses UELoggingModule for error and state output

***

## Installation

### Prerequisites

- Unreal Engine 4.27+ or UE5
- Visual Studio 2019/2022 (Windows), Xcode (Mac), or equivalent toolchain
- C++20 standard enabled in your target Unreal project

### Setup Steps

1. **Copy Module:** Place the `BlueprintFutureModule` folder into your project's `Source` directory.
2. **Generate Project Files:** Right-click on your `.uproject` and select "Generate Visual Studio project files".
3. **Rebuild:** Build the project in your IDE or via Unreal Editor to ensure the module is compiled.

***

## Usage

### Creating and Handling Futures in C++

Example usage chaining async operations and creating promises/futures:

```cpp
// Create a promise and get its future
UBlueprintPromiseHandle* Promise = UBlueprintPromiseHandle::MakePromise();
UBlueprintFutureHandle* Future = Promise->GetFuture();

// Fulfill the promise later
Promise->SetValue(...);

// Chain async operations
Future->Then([](UBlueprintFutureHandle* CompletedFuture){
    // Do something when complete
});
```

### Combining Multiple Futures ("Bunch" Operation)

```cpp
TArray<UBlueprintFutureHandle*> Futures = { FutureA, FutureB, FutureC };
UBlueprintFutureHandle* BunchFuture = UBlueprintFutureFunctionLibrary::ThenBunch(Futures);
```

### Supported Pod Types

Supports passing values of these types:
- bool, int32, float, FString, FText

Example for creating a supported type:

```cpp
// Make a pod-type supported variant
FSupportedPodType BoolValue = UPodTypeFunctionLibrary::MakeSupportedPodTypeBool(true);
```

***

### Blueprint Usage

Blueprint nodes are provided for all major async operations.

#### Creating a Promise & Waiting on a Future

<img width="1209" height="400" alt="Captura de pantalla 2025-10-24 a las 6 53 28 p  m" src="https://github.com/user-attachments/assets/ba7a11c6-e235-46b0-8f3b-e83e63cb6337" />

<img width="931" height="603" alt="Captura de pantalla 2025-10-24 a las 6 55 35 p  m" src="https://github.com/user-attachments/assets/a51330d5-798b-485a-9c6b-0d182a2c0d99" />

<img width="936" height="480" alt="Captura de pantalla 2025-10-24 a las 6 57 17 p  m" src="https://github.com/user-attachments/assets/09cbceb4-d2d6-423c-a13d-47f51f1adddf" />


#### Chaining Future Operations

<img width="1120" height="392" alt="Captura de pantalla 2025-10-24 a las 7 01 48 p  m" src="https://github.com/user-attachments/assets/46517f81-9734-40c1-a751-402fd75afea2" />

#### Combining Multiple Futures

<img width="582" height="447" alt="Captura de pantalla 2025-10-24 a las 7 02 56 p  m" src="https://github.com/user-attachments/assets/e92c4830-473a-492f-8d5f-a96461ee7e19" />

***

## API Documentation

### Core Classes

- **UBlueprintPromiseHandle**
  - `SetValue(FSupportedTypesVariant Value)`
  - `SetEmptyValue()`
  - `GetFuture()`
- **UBlueprintFutureHandle**
  - `Wait()`
  - `WaitForTimespan(FTimespan Timespan)`
  - `Then(TFunction<void(UBlueprintFutureHandle*)>)`
  - `Next(TFunction<UBlueprintFutureHandle*(FSupportedTypesVariant)>)`
  - `GetValue()`
- **UBlueprintFutureFunctionLibrary**
  - `ThenBunch(TArray<UBlueprintFutureHandle*> Futures) : UBlueprintFutureHandle*`
- **UPodTypeFunctionLibrary**
  - `MakeSupportedPodTypeBool(bool Value)`
  - `MakeSupportedPodTypeInt(int32 Value)`
  - `MakeSupportedPodTypeFloat(float Value)`
  - `MakeSupportedPodTypeString(FString Value)`
  - `MakeSupportedPodTypeText(FText Value)`

### Blueprint Nodes

All major async and value-passing methods (promise creation, fulfillment, chaining, bunching, pod/variant making, handle movement) are exposed as Blueprint nodes under `"BlueprintFuture"` and `"Future Utilities"` categories.

***

## Contributing

1. **Fork the repository** and clone to your machine.
2. **Write clean, consistent C++ code** following Unreal Engine's style guide.
3. For any features or fixes, open a **pull request** with a clear description.
4. Add or update documentation and Blueprint node samples in the README.
5. Ensure your code builds and passes all UE4/5 compile-time checks.
6. Adhere to existing project structure and commit standards (conventional commits, descriptive messages).

***
