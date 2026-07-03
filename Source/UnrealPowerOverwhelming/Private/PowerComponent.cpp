// <copyright file="PowerComponent.coo" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "PowerComponent.h"

#include <visus/pwrowg/dump_sensors.h>
#include <visus/pwrowg/marker_configuration.h>

#define LOCTEXT_NAMESPACE "UPowerComponent"



/*
 * UPowerComponent::UPowerComponent
 */
UPowerComponent::UPowerComponent(void)
        : MarkTicks(false),
        OutputPath(TEXT("unreal-power.csv")),
        SampleBufferSize(1024),
        SampleInterval(5),
        SensorPath(TEXT("unreal-power.json")),
        _markers(nullptr) {
    PrimaryComponentTick.bCanEverTick = true;
}


/*
 * UPowerComponent::EmitMarker
 */
bool UPowerComponent::EmitMarker(const int id) {
    if (this->_markers == nullptr) {
        return false;
    }

    return this->_markers->emit(id);
}


/*
 * UPowerComponent::TickComponent
 */
void UPowerComponent::TickComponent(float dt,
        ELevelTick type,
        FActorComponentTickFunction *function) {
    Super::TickComponent(dt, type, function);

    if (this->_markers != nullptr) {
        this->_markers->emit();
    }
}


/*
 * UPowerComponent::BeginPlay
 */
void UPowerComponent::BeginPlay(void) {
    Super::BeginPlay();

    UE_LOG(LogTemp, Verbose, TEXT("Configuring Power Overwhelming sensors."));
    visus::pwrowg::sensor_array_configuration config;

    for (auto& d : this->DisabledSensorClasses) {
        try {
            config.exclude(visus::pwrowg::guid::parse(*d));
        } catch (...) {
            continue;
        }
    }

    this->_sink = MakeShared<visus::pwrowg::thread_local_sink<FPowerSink>>(
        this->SampleBufferSize, this->OutputPath);

    config.sample_every(std::abs(this->SampleInterval))
        .deliver_to(decltype(this->_sink)::ElementType::sample_callback)
        .deliver_context(this->_sink.Get());

    UE_LOG(LogTemp, Verbose, TEXT("Creating Power Overwhelming sensor array."));
    this->_sensors = visus::pwrowg::sensor_array::for_all(std::move(config));

    this->_markers = this->MarkTicks
        ? this->_sensors.controller<visus::pwrowg::marker_configuration>()
        : nullptr;

    UE_LOG(LogTemp, Verbose, TEXT("Dumping sensor details."));
    visus::pwrowg::dump_sensors(this->_sensors, *this->SensorPath);

    UE_LOG(LogTemp, Verbose, TEXT("Starting Power Overwhelming sensor array."));
    this->_sensors.start();
}


/*
 * UPowerComponent::EndPlay
 */
void UPowerComponent::EndPlay(const EEndPlayReason::Type reason) {
    Super::EndPlay(reason);
    UE_LOG(LogTemp, Verbose, TEXT("Stopping Power Overwhelming sensor array."));
    this->_sensors.stop();
}

#undef LOCTEXT_NAMESPACE





