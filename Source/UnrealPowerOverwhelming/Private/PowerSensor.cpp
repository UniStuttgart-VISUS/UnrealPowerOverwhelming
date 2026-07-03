//// <copyright file="PowerSensor.h" company="Visualisierungsinstitut der Universität Stuttgart">
//// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
//// Licensed under the MIT licence. See LICENCE file for details.
//// </copyright>
//// <author>Christoph Müller</author>
//
//#include "PowerSensor.h"
//
//
///*
// * UPowerSensor::UPowerSensor
// */
//UPowerSensor::UPowerSensor(const visus::pwrowg::sensor_description& sensor)
//        : _sensor(sensor) { }
//
//
///*
// * UPowerSensor::~UPowerSensor
// */
//UPowerSensor::~UPowerSensor(void) noexcept { }
//
//
///*
// * UPowerSensor::GetID
// */
//FString UPowerSensor::GetID(void) const {
//    return FString(this->_sensor.id());
//}
//
//
///*
// * UPowerSensor::GetName
// */
//FString UPowerSensor::GetName(void) const {
//    return FString(this->_sensor.name());
//}
//
//
///*
// * UPowerSensor::GetVendor
// */
//FString UPowerSensor::GetVendor(void) const {
//    return FString(this->_sensor.vendor());
//}
