// <copyright file="PowerComponent.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "PowerComponent.h"

#include <visus/pwrowg/convert_string.h>
#include <visus/pwrowg/daqmx_configuration.h>
#include <visus/pwrowg/dump_sensors.h>
#include <visus/pwrowg/hmc8015_configuration.h>
#include <visus/pwrowg/marker_configuration.h>
#include <visus/pwrowg/msr_configuration.h>
#include <visus/pwrowg/powenetics_configuration.h>
#include <visus/pwrowg/rtx_configuration.h>
#include <visus/pwrowg/sensor_filters.h>
#include <visus/pwrowg/tinkerforge_configuration.h>
#include <visus/pwrowg/usb_pd_configuration.h>

#define LOCTEXT_NAMESPACE "UPowerComponent"

#include "UnrealPowerOverwhelming.h"


/*
 * UPowerComponent::UPowerComponent
 */
UPowerComponent::UPowerComponent(void)
        : MarkTicks(false),
        OutputPath(TEXT("unreal-power.csv")),
        PoweneticsTimeout(1000),
        RaplOnlyCore0(false),
        Required(false),
        SampleBufferSize(1024),
        SampleInterval(5),
        SensorPath(TEXT("unreal-power.json")),
        TinkerforgeCurrentConversionTime(3),
        TinkerforgeSampleAveraging(1),
        TinkerforgeTimeout(1000),
        TinkerforgeVoltageConversionTime(3),
        UsbPdConnectTimeout(1000),
        UsbPdReadTimeout(5),
        UsbPdWriteTimeout(100),
        _markers(nullptr) {
    PrimaryComponentTick.bCanEverTick = true;

    // Disable uncommon sensors which are slow to load by default.
    {
        auto i = visus::pwrowg::hmc8015_configuration::id.to_string<TCHAR>();
        this->DisabledSensorClasses.Add(i.c_str());
    }

    {
        // 0xebfa0324, 0x7d7a, 0x464e, 0x85, 0x10, 0xc1, 0xa1, 0x9c, 0xba, 0xac, 0xaf
        auto i = visus::pwrowg::rtx_configuration::id.to_string<TCHAR>();
        this->DisabledSensorClasses.Add(i.c_str());
    }

    {
        // 0x2A24F596, 0xC0C0, 0x4395, 0x81CD, 0x92, 0x42, 0xFD, 0xA9, 0xAC, 0xF1
        auto i = visus::pwrowg::tinkerforge_configuration::id.to_string<TCHAR>();
        this->DisabledSensorClasses.Add(i.c_str());
    }

    {
        auto i = visus::pwrowg::usb_pd_configuration::id.to_string<TCHAR>();
        this->DisabledSensorClasses.Add(i.c_str());
    }

    {
        visus::pwrowg::tinkerforge_configuration::end_point e;
        auto h = visus::pwrowg::convert_string<FString::ElementType>(e.name());
        this->TinkerforgeEndPoint = h.c_str();
        this->TinkerforgeEndPoint.AppendChar(TEXT(':'));
        this->TinkerforgeEndPoint.AppendInt(e.port());
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
 * UPowerComponent::Start
 */
bool UPowerComponent::Start(void) {
    if (!this->CanMeasure()) {
        UE_LOG(LogPwrOwg, Verbose, TEXT("Power Overwhelming sensors cannot be ")
            TEXT("started from a class default object."));
        return false;
    }

    try {
        UE_LOG(LogPwrOwg, Verbose, TEXT("Opening power output file %s."),
            *this->OutputPath);
        if (!this->_sink->Open(this->OutputPath)) {
            UE_LOG(LogPwrOwg, Error, TEXT("Failed to open power output file %s."),
                *this->OutputPath);
            return false;
        }

        UE_LOG(LogPwrOwg, Verbose, TEXT("Dumping sensor details to %s."),
            *this->SensorPath);
        visus::pwrowg::dump_sensors(this->_sensors, *this->SensorPath);

        UE_LOG(LogPwrOwg, Verbose, TEXT("Starting Power Overwhelming sensor ")
            TEXT("array in %s (%d)."), *this->GetFName().ToString(),
            this->GetUniqueID());
        this->_sensors.start();
        return true;
    } catch (std::exception& ex) {
        UE_LOG(LogPwrOwg, Warning, TEXT("Failed to start Power Overwhelming ")
            TEXT("sensors: %hs"), ex.what());
        return false;
    } catch (...) {
        UE_LOG(LogPwrOwg, Warning, TEXT("Failed to start Power Overwhelming ")
            TEXT("sensors."));
        return false;
    }
}


/*
 * UPowerComponent::Stop
 */
bool UPowerComponent::Stop(void) {
    if (!this->CanMeasure()) {
        UE_LOG(LogPwrOwg, Verbose, TEXT("Power Overwhelming sensors cannot be ")
            TEXT("stopped from a class default object."));
        return false;
    }

    try {
        UE_LOG(LogPwrOwg, Verbose, TEXT("Stopping Power Overwhelming sensor ")
            TEXT("array in %s (%d)."), *this->GetFName().ToString(),
            this->GetUniqueID());
        this->_sensors.stop();
        return true;
    } catch (std::exception& ex) {
        UE_LOG(LogPwrOwg, Warning, TEXT("Failed to stop Power Overwhelming ")
            TEXT("sensors: %hs"), ex.what());
        return false;
    } catch (...) {
        UE_LOG(LogPwrOwg, Warning, TEXT("Failed to stop Power Overwhelming ")
            TEXT("sensors."));
        return false;
    }
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
bool UPowerComponent::Trigger() {
    bool retval = false;

    if (!this->CanMeasure()) {
        UE_LOG(LogPwrOwg, Verbose, TEXT("Power Overwhelming sensors cannot be ")
            TEXT("triggered from a class default object."));
        return false;
    }

    UE_LOG(LogPwrOwg, Verbose, TEXT("Triggering oscilloscope and DAQ sensors ")
        TEXT("in %s (%d)."), *this->GetFName().ToString(), this->GetUniqueID());

    try {
        retval = this->_rtx_trigger.acquire(
            [](void) {
                UE_LOG(LogPwrOwg, Verbose, TEXT("RTx sample acquired."));
            },
            [](void) {
                UE_LOG(LogPwrOwg, Verbose, TEXT("RTx sample processed."));
            },
            [](std::exception_ptr) {
                UE_LOG(LogPwrOwg, Warning, TEXT("RTx acquisition failed."));
                return true;
            }) || retval;
    } catch (...) {
        UE_LOG(LogPwrOwg, Warning, TEXT("Failed triggering RTx sensors."));
    }

    try {
        retval = this->_daqmx_trigger.acquire(
            [](void) {
                UE_LOG(LogPwrOwg, Verbose, TEXT("NI-DAQmx sample acquired."));
            },
            [](std::exception_ptr) {
                UE_LOG(LogPwrOwg, Warning, TEXT("NI-DAQmx acquisition failed."));
                return true;
            }) || retval;
    } catch (...) {
        UE_LOG(LogPwrOwg, Warning, TEXT("Failed triggering NI-DAQmx sensors."));
    }

    if (retval) {
        UE_LOG(LogPwrOwg, Verbose, TEXT("At least one sensor was triggered by ")
            TEXT("%s (%d)."), *this->GetFName().ToString(), this->GetUniqueID());
    }

    return retval;
}


/*
 * UPowerComponent::BeginPlay
 */
void UPowerComponent::BeginPlay(void) {
    Super::BeginPlay();

    if (!this->CanMeasure()) {
        UE_LOG(LogPwrOwg, Verbose, TEXT("Power Overwhelming sensors cannot be ")
            TEXT("configured in a class default object."));
        return;
    }

    UE_LOG(LogPwrOwg, Verbose, TEXT("Configuring Power Overwhelming sensors ")
        TEXT("in %s (%d) on game startup."), *this->GetFName().ToString(),
        this->GetUniqueID());
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
        this->SampleBufferSize, true);

    config.sample_every(std::abs(this->SampleInterval))
        .deliver_to(decltype(this->_sink)::ElementType::sample_callback)
        .deliver_context(this->_sink.Get());

    if (FPaths::FileExists(this->DaqmxConfiguration)) {
        UE_LOG(LogPwrOwg, Verbose, TEXT("Configuring DAQmx sensors from JSON."));
        config.configure<visus::pwrowg::daqmx_configuration>(
            [this](visus::pwrowg::daqmx_configuration& c) {
                c = visus::pwrowg::daqmx_configuration::load(
                    *this->DaqmxConfiguration);
                this->_daqmx_trigger = c.trigger();
            });
    }

    config.configure<visus::pwrowg::powenetics_configuration>(
        [this](visus::pwrowg::powenetics_configuration& c) {
            c.timeout(std::chrono::milliseconds(this->PoweneticsTimeout));
        });

    config.configure<visus::pwrowg::msr_configuration>(
        [this](visus::pwrowg::msr_configuration& c) {
            c.first_core(this->RaplOnlyCore0);
        });

    if (FPaths::FileExists(this->RtxConfiguration)) {
        UE_LOG(LogPwrOwg, Verbose, TEXT("Configuring RTx sensors from JSON."));
        config.configure<visus::pwrowg::rtx_configuration>(
            [this](visus::pwrowg::rtx_configuration& c) {
                c = visus::pwrowg::rtx_configuration::load(
                    *this->RtxConfiguration);
                this->_rtx_trigger = c.trigger();
            });
    }

    if (!this->TinkerforgeEndPoint.IsEmpty()) {
        typedef visus::pwrowg::tinkerforge_sample_averaging averaging;
        typedef visus::pwrowg::tinkerforge_conversion_time conversion;
        typedef visus::pwrowg::tinkerforge_configuration::end_point end_point;
        config.configure<visus::pwrowg::tinkerforge_configuration>(
            [this](visus::pwrowg::tinkerforge_configuration& c) {
                auto e = end_point::parse(*this->TinkerforgeEndPoint);
                c.end_points(&e, 1);
                c.averaging(static_cast<averaging>(
                    this->TinkerforgeSampleAveraging));
                c.current_conversion_time(static_cast<conversion>(
                    this->TinkerforgeCurrentConversionTime));
                c.voltage_conversion_time(static_cast<conversion>(
                    this->TinkerforgeVoltageConversionTime));
                c.timeout(std::chrono::milliseconds(this->TinkerforgeTimeout));
            });
    }

    config.configure<visus::pwrowg::usb_pd_configuration>(
        [this](visus::pwrowg::usb_pd_configuration& c) {
            c.read_timeout(std::chrono::milliseconds(this->UsbPdReadTimeout));
            c.timeout(std::chrono::milliseconds(this->UsbPdConnectTimeout));
            c.write_timeout(std::chrono::milliseconds(this->UsbPdWriteTimeout));
        });

    UE_LOG(LogPwrOwg, Verbose, TEXT("Creating Power Overwhelming sensor array ")
        TEXT("in %s (%d)."), *this->GetFName().ToString(), this->GetUniqueID());
    try {
        this->_sensors = visus::pwrowg::sensor_array::for_matches(
            std::move(config), visus::pwrowg::is_any_of<
            visus::pwrowg::is_power_sensor,
            visus::pwrowg::is_marker_sensor,
            visus::pwrowg::is_voltage_sensor,
            visus::pwrowg::is_current_sensor>);
        for (auto& s : this->_sensors) {
            UE_LOG(LogPwrOwg, Verbose, TEXT("Power sensor %s is active."),
                s.id());
        }
    } catch (std::exception& ex) {
        UE_LOG(LogPwrOwg, Error, TEXT("Failed to create Power Overwhelming ")
            TEXT("sensors: %hs"), ex.what());
        if (this->Required) {
            throw;
        }
    } catch (...) {
        UE_LOG(LogPwrOwg, Error, TEXT("Failed to create Power Overwhelming ")
            TEXT("sensors."));
        if (this->Required) {
            throw;
        }
    }

    this->_markers = this->MarkTicks
        ? this->_sensors.controller<visus::pwrowg::marker_configuration>()
        : nullptr;
}


/*
 * UPowerComponent::EndPlay
 */
void UPowerComponent::EndPlay(const EEndPlayReason::Type reason) {
    Super::EndPlay(reason);

    UE_LOG(LogPwrOwg, Verbose, TEXT("Making sure that Power Overwhelming ")
        TEXT("sensors in %s (%d) are not running anymore."),
        *this->GetFName().ToString(), this->GetUniqueID());
    if (!this->Stop()) {
        UE_LOG(LogPwrOwg, Display, TEXT("The Power Overwhelming sensor ")
            TEXT("array could not be stopped as part of the component ")
            TEXT("shutdown. This problem can be ignored if the sensors are ")
            TEXT("managed manually (i.e. the array has not been running when ")
            TEXT("the game stopped."));
    }
}


/*
 * UPowerComponent::CanMeasure
 */
bool UPowerComponent::CanMeasure(void) const {
    return !this->HasAnyFlags(RF_ClassDefaultObject
        | RF_ImmutableDefaultObject);
}

#undef LOCTEXT_NAMESPACE
