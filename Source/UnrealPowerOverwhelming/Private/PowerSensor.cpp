// <copyright file="PowerSensor.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "PowerSensor.h"


/*
 * FPowerSensor::FPowerSensor
 */
FPowerSensor::FPowerSensor(const visus::pwrowg::sensor_description& sensor)
        : ID(FString(sensor.id())),
        Name(FString(sensor.name())),
        Vendor(FString(sensor.vendor())),
        _sensor(sensor) {
    auto id = sensor.sensor_class().to_string<TCHAR>();
    this->Class = FString(id.c_str());
}
