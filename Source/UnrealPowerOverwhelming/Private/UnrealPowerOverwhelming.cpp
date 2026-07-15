// <copyright file="UnrealPowerOverwhelming.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "UnrealPowerOverwhelming.h"

#include "Interfaces/IPluginManager.h"


#define LOCTEXT_NAMESPACE "FUnrealPowerOverwhelmingModule"


/*
 * FUnrealPowerOverwhelmingModule::FUnrealPowerOverwhelmingModule
 */
FUnrealPowerOverwhelmingModule::FUnrealPowerOverwhelmingModule(void) noexcept
    : _nvapi(nullptr) { }


/*
 * FUnrealPowerOverwhelmingModule::ShutdownModule
 */
void FUnrealPowerOverwhelmingModule::ShutdownModule(void) {
    if (this->_nvapi != nullptr) {
        FPlatformProcess::FreeDllHandle(this->_nvapi);
        this->_nvapi = nullptr;
    }
}


/*
 * FUnrealPowerOverwhelmingModule::StartupModule
 */
void FUnrealPowerOverwhelmingModule::StartupModule(void) {
    auto& plugins = IPluginManager::Get();
    auto plugin = plugins.FindPlugin(TEXT("UnrealPowerOverwhelming"));

    if (plugin != nullptr) {
        auto path = FPaths::Combine(
            plugin->GetBaseDir(),
            TEXT("Binaries"),
            FPlatformProcess::GetBinariesSubdirectory(),
            TEXT("pwrovrnvapi.dll"));
        this->_nvapi = FPlatformProcess::GetDllHandle(*path);
    }
}

#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(FUnrealPowerOverwhelmingModule, UnrealPowerOverwhelming)
