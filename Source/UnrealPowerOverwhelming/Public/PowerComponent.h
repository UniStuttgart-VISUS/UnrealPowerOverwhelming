// <copyright file="PowerComponent.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <visus/pwrowg/marker_controller.h>
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

    // Emits a marker with the specified ID value.
    UFUNCTION(BlueprintCallable)
    bool EmitMarker(const int id);

    /// <inheritdoc />
    virtual void TickComponent(float dt,
        ELevelTick type,
        FActorComponentTickFunction *function) override;

protected:

    /// <inheritdoc />
    virtual void BeginPlay() override;

    /// <inheritdoc />
    virtual void EndPlay(const EEndPlayReason::Type reason) override;

private:

    visus::pwrowg::marker_controller *_markers;
    visus::pwrowg::sensor_array _sensors;
    TSharedPtr<visus::pwrowg::thread_local_sink<FPowerSink>> _sink;
};
