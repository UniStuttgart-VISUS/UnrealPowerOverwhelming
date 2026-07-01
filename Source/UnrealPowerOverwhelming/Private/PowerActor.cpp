// <copyright file="PowerActor.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "PowerActor.h"


/*
 * APowerActor::APowerActor
 */
APowerActor::APowerActor(void) {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}


/*
 * APowerActor::Tick
 */
void APowerActor::Tick(float dt) {
    Super::Tick(dt);
}


/*
 * APowerActor::BeginPlay
 */
void APowerActor::BeginPlay(void) {
    Super::BeginPlay();

    visus::pwrowg::sensor_array_configuration config;
}
