// <copyright file="PowerComponent.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <visus/pwrowg/daqmx_sensor_trigger.h>
#include <visus/pwrowg/marker_controller.h>
#include <visus/pwrowg/rtx_sensor_trigger.h>
#include <visus/pwrowg/sensor_array.h>
#include <visus/pwrowg/thread_local_sink.h>

#include "Components/ActorComponent.h"

#include "CoreMinimal.h"
#include "PowerSink.h"

#include "PowerComponent.generated.h"


// A component that provides power measurement capabilities to an actor.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPOWEROVERWHELMING_API UPowerComponent : public UActorComponent {
    GENERATED_BODY()

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    UPowerComponent();

    // Gets or sets the path to a JSON file that contains the configuration of
    // the NI-DAQmx sensors.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    FString DaqmxConfiguration;

    // Gets or sets the GUIDs of the sensor classes that will not be enumerated
    // when the sensor array is created.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    TArray<FString> DisabledSensorClasses;

    // Emits a marker for each tick.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    bool MarkTicks;

    // Gets or sets the path to the file where the power sensor data will be
    // saved to.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Output")
    FString OutputPath;

    // Gets or sets the timeout for the Powenetics sensor in milliseconds.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    int32 PoweneticsTimeout;

    // Gets or sets the path to a JSON file that contains the configuration of
    // the Rohde & Schwarz RTA/RTB sensors.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    FString RtxConfiguration;

    // Gets or sets the number of samples that will be buffered before writing
    // to the output file.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Output")
    int SampleBufferSize;

    // Gets or sets the time between power samples in milliseconds.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    int SampleInterval;

    // Gets or sets the path to a file where the sensor configuration will be
    // logged.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Output")
    FString SensorPath;

    // Gets or sets whether the sensor array should start with the game.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    bool StartWithGame;

    // Gets or sets the current conversion time of the Tinkerforge sensor. Note
    // that this is an enumerated value which can be looked up in the
    // Tinkerforge documentation.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    uint8 TinkerforgeCurrentConversionTime;

    // Gets or sets the location of the Tinkerforge end point.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    FString TinkerforgeEndPoint;

    // Gets or sets how many hardware samples the Tinkerforge sensor should
    // average to obtain an output sample. Note that this is an enumerated
    // value which can be looked up in the Tinkerforge documentation.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    uint8 TinkerforgeSampleAveraging;

    // Gets or sets the timeout for the Tinkerforge end point in milliseconds.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    int64 TinkerforgeTimeout;

    // Gets or sets the voltage conversion time of the Tinkerforge sensor. Note
    // that this is an enumerated value which can be looked up in the
    // Tinkerforge documentation.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    uint8 TinkerforgeVoltageConversionTime;

    // Gets or sets the timeout, in milliseconds, for connection to the USB-PD
    // sensor.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    int32 UsbPdConnectTimeout;

    // Gets or sets the timeout, in milliseconds, for reading from the USB-PD
    // sensor.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    int32 UsbPdReadTimeout;

    // Gets or sets the timeout, in milliseconds, for writing to the USB-PD
    // sensor.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sensors")
    int32 UsbPdWriteTimeout;

    // Emits a marker with the specified ID value.
    UFUNCTION(BlueprintCallable)
    bool EmitMarker(const int id);

    // Tries starting the sensor array.
    UFUNCTION(BlueprintCallable)
    bool Start();

    // Tries stopping the sensor array.
    UFUNCTION(BlueprintCallable)
    bool Stop();

    /// <inheritdoc />
    virtual void TickComponent(float dt,
        ELevelTick type,
        FActorComponentTickFunction *function) override;

    // Triggers manual sensors like oscilloscopes.
    UFUNCTION(BlueprintCallable)
    bool Trigger();

protected:

    /// <inheritdoc />
    virtual void BeginPlay() override;

    /// <inheritdoc />
    virtual void EndPlay(const EEndPlayReason::Type reason) override;

private:

    visus::pwrowg::daqmx_sensor_trigger _daqmx_trigger;
    visus::pwrowg::marker_controller *_markers;
    visus::pwrowg::rtx_sensor_trigger _rtx_trigger;
    visus::pwrowg::sensor_array _sensors;
    TSharedPtr<visus::pwrowg::thread_local_sink<FPowerSink>> _sink;
};
