// <copyright file="PowerFunctions.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CoreMinimal.h"
#include "PowerComponent.h"
#include "PowerSensor.h"

#include "PowerFunctions.generated.h"


/// <summary>
/// A blueprint function library for interacting with Power Overwhelming.
/// </summary>
UCLASS(Category = "Power Overwhelming")
class UNREALPOWEROVERWHELMING_API UPowerFunctions final
        : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:

    // Gets the class IDs of all sensors that are currently available to the
    // Power Overwhelming plugin under the default configuration.
    UFUNCTION(BlueprintCallable, Category = "Power Overwhelming")
    static TSet<FString> GetSensorClassIDs();

    // Gets the all sensors that are currently available to the Power
    // Overwhelming plugin under the default configuration.
    UFUNCTION(BlueprintCallable, Category = "Power Overwhelming")
    static TArray<FPowerSensor> GetSensors();

};
