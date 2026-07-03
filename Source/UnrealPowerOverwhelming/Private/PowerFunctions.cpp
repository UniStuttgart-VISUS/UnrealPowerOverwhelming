// <copyright file="PowerFunctions.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "PowerFunctions.h"

#include <visus/pwrowg/sensor_array.h>


/*
 * UPowerFunctions::GetSensorClassIDs
 */
TSet<FString> UPowerFunctions::GetSensorClassIDs(void) {
    visus::pwrowg::sensor_array_configuration config;
    TArray<visus::pwrowg::sensor_description> sensors;

    sensors.SetNum(visus::pwrowg::sensor_array::all_descriptions(
        nullptr, 0, config));
    visus::pwrowg::sensor_array::all_descriptions(
        sensors.GetData(), sensors.Num(), config);

    TSet<FString> retval;
    for (auto& s : sensors) {
        auto i = s.sensor_class().to_string<TCHAR>();
        retval.Add(FString(i.c_str()));
    }

    return retval;
}



/*
 * UPowerFunctions::GetSensorIDs
 */
TArray<FString> UPowerFunctions::GetSensorIDs(void) {
    visus::pwrowg::sensor_array_configuration config;
    TArray<visus::pwrowg::sensor_description> sensors;

    sensors.SetNum(visus::pwrowg::sensor_array::all_descriptions(
        nullptr, 0, config));
    visus::pwrowg::sensor_array::all_descriptions(
        sensors.GetData(), sensors.Num(), config);

    TArray<FString> retval;
    for (auto& s : sensors) {
        retval.Add(FString(s.id()));
    }

    return retval;
}
