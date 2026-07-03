//// <copyright file="PowerSensor.h" company="Visualisierungsinstitut der Universität Stuttgart">
//// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
//// Licensed under the MIT licence. See LICENCE file for details.
//// </copyright>
//// <author>Christoph Müller</author>
//
//#pragma once
//
//#include <visus/pwrowg/sensor_description.h>
//
//#include "CoreMinimal.h"
//
//#include "PowerSensor.generated.h"
//
//
//// The descriptor of a power sensor.
//UCLASS()
//class UNREALPOWEROVERWHELMING_API UPowerSensor final : public UClass {
//    GENERATED_BODY()
//
//public:
//
//    /// <summary>
//    /// Initialises a new instance.
//    /// </summary>
//    explicit UPowerSensor(const visus::pwrowg::sensor_description& sensor);
//
//    /// <summary>
//    /// Finalises the instance.
//    /// </summary>
//    ~UPowerSensor(void) noexcept;
//
//    // Gets the unique identifier of the sensor.
//    UFUNCTION(BlueprintCallable, Category = "Power Overwhelming")
//    FString GetID() const;
//
//    // Gets the friendly name of the sensor.
//    UFUNCTION(BlueprintCallable, Category = "Power Overwhelming")
//    FString GetName() const;
//
//    // Gets the vendor of the sensor.
//    UFUNCTION(BlueprintCallable, Category = "Power Overwhelming")
//    FString GetVendor() const;
//
//    /// <summary>
//    /// Exposes the native descriptor.
//    /// </summary>
//    /// <returns>The native descriptor.</returns>
//    inline operator const visus::pwrowg::sensor_description&(
//            void) const noexcept {
//        return this->_sensor;
//    }
//
//private:
//
//    visus::pwrowg::sensor_description _sensor;
//};
