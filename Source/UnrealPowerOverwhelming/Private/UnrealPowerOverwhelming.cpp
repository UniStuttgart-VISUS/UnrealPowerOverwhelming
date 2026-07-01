// <copyright file="UnrealPowerOverwhelming.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "UnrealPowerOverwhelming.h"


#define LOCTEXT_NAMESPACE "FUnrealPowerOverwhelmingModule"

/*
 * FUnrealPowerOverwhelmingModule::ShutdownModule
 */
void FUnrealPowerOverwhelmingModule::ShutdownModule(void) {
    // This function may be called during shutdown to clean up your module. For
    // modules that support dynamic reloading, we call this function before
    // unloading the module.
}


/*
 * FUnrealPowerOverwhelmingModule::StartupModule
 */
void FUnrealPowerOverwhelmingModule::StartupModule(void) {
    // This code will execute after your module is loaded into memory; the
    // exact timing is specified in the .uplugin file per-module
}

#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(FUnrealPowerOverwhelmingModule, UnrealPowerOverwhelming)
