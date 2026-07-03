// <copyright file="PowerSensor.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <visus/pwrowg/sensor_description.h>

#include "CoreMinimal.h"

#include "PowerSensor.generated.h"


// The descriptor of a power sensor.
USTRUCT(BlueprintType, Category = "Power Overwhelming")
struct UNREALPOWEROVERWHELMING_API FPowerSensor final {
    GENERATED_BODY()

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    FPowerSensor(void) = default;

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    explicit FPowerSensor(const visus::pwrowg::sensor_description& sensor);

    // Gets the class ID of the sensor.
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Power Overwhelming")
    FString Class;

    // Gets the unique identifier of the sensor.
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Power Overwhelming")
    FString ID;

    // Gets the friendly name of the sensor.
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Power Overwhelming")
    FString Name;

    // Gets the vendor of the sensor.
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Power Overwhelming")
    FString Vendor;

    /// <summary>
    /// Exposes the native descriptor.
    /// </summary>
    /// <returns>The native descriptor.</returns>
    inline operator const visus::pwrowg::sensor_description&(
            void) const noexcept {
        return this->_sensor;
    }

private:

    visus::pwrowg::sensor_description _sensor;
};
