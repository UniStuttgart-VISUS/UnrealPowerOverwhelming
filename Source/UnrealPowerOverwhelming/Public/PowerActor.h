// <copyright file="PowerActor.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "visus/pwrowg/sensor_array.h"

#include "PowerActor.generated.h"


UCLASS()
class UNREALPOWEROVERWHELMING_API APowerActor : public AActor {
    GENERATED_BODY()

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    APowerActor();

    /// <summary>
    /// Gets or sets the GUIDs of the sensor classes that will not be enumerated
    /// when the sensor array is created.
    /// </summary>
    UPROPERTY(BlueprintReadWrite, Category = "Power")
    TArray<FString> DisabledSensorClasses;

    /// <summary>
    /// Gets or sets the path to the file where the power sensor data will be
    /// saved to.
    /// </summary>
    UPROPERTY(BlueprintReadWrite, Category = "Power")
    FString OutputPath;

    /// <inheritdoc />
    virtual void Tick(float dt) override;

protected:

    /// <inheritdoc />
    virtual void BeginPlay() override;

private:

    //visus::pwrowg::sensor_array _sensors;

};
