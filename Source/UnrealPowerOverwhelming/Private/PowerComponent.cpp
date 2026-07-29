// <copyright file="PowerComponent.coo" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "PowerComponent.h"

#include <visus/pwrowg/daqmx_configuration.h>
#include <visus/pwrowg/dump_sensors.h>
#include <visus/pwrowg/hmc8015_configuration.h>
#include <visus/pwrowg/marker_configuration.h>
#include <visus/pwrowg/rtx_configuration.h>
#include <visus/pwrowg/tinkerforge_configuration.h>
#include <visus/pwrowg/usb_pd_configuration.h>

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

    // Disable uncommon sensors which are slow to load by default.
    {
        auto i = visus::pwrowg::hmc8015_configuration::id.to_string<TCHAR>();
        this->DisabledSensorClasses.Add(i.c_str());
    }

    {
        auto i = visus::pwrowg::rtx_configuration::id.to_string<TCHAR>();
        this->DisabledSensorClasses.Add(i.c_str());
    }

    {
        auto i = visus::pwrowg::tinkerforge_configuration::id.to_string<TCHAR>();
        this->DisabledSensorClasses.Add(i.c_str());
    }

    {
        auto i = visus::pwrowg::usb_pd_configuration::id.to_string<TCHAR>();
        this->DisabledSensorClasses.Add(i.c_str());
    }
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
 * UPowerComponent::Trigger
 */
UFUNCTION(BlueprintCallable)
bool UPowerComponent::Trigger() {
    bool retval = false;

    try {
        retval = this->_rtx_trigger.acquire(
            [](void) {
                UE_LOG(LogTemp, Verbose, TEXT("RTx sample acquired."));
            },
            [](std::exception_ptr) {
                UE_LOG(LogTemp, Warning, TEXT("RTx acquisition failed."));
                return true;
            }) || retval;
    } catch (...) {
        UE_LOG(LogTemp, Warning, TEXT("Failed triggering RTx sensors."));
    }

    try {
        retval = this->_daqmx_trigger.acquire(
            [](void) {
                UE_LOG(LogTemp, Verbose, TEXT("NI-DAQmx sample acquired."));
            },
            [](std::exception_ptr) {
                UE_LOG(LogTemp, Warning, TEXT("NI-DAQmx acquisition failed."));
                return true;
            }) || retval;
    } catch (...) {
        UE_LOG(LogTemp, Warning, TEXT("Failed triggering NI-DAQmx sensors."));
    }

    return retval;
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
            auto i = visus::pwrowg::guid::parse(*d);
            config.exclude(i);
        } catch (...) {
            continue;
        }
    }

    this->_sink = MakeShared<visus::pwrowg::thread_local_sink<FPowerSink>>(
        this->SampleBufferSize, this->OutputPath);

    config.sample_every(std::abs(this->SampleInterval))
        .deliver_to(decltype(this->_sink)::ElementType::sample_callback)
        .deliver_context(this->_sink.Get());

    if (FPaths::FileExists(this->DaqmxConfiguration)) {
        //config.load_daqmx(*this->DaqmxConfiguration);
    }

    if (FPaths::FileExists(this->RtxConfiguration)) {
        UE_LOG(LogTemp, Verbose, TEXT("Configuring RTx sensors from JSON."));
        config.configure<visus::pwrowg::rtx_configuration>(
            [this](visus::pwrowg::rtx_configuration& c) {
                c = visus::pwrowg::rtx_configuration::load(
                    *this->RtxConfiguration);
            });
    }

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





