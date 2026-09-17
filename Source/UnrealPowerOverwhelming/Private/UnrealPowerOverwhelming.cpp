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
    : _nvapi(nullptr), _pwrowg(nullptr) { }


/*
 * FUnrealPowerOverwhelmingModule::ShutdownModule
 */
void FUnrealPowerOverwhelmingModule::ShutdownModule(void) {
    if (this->_benchlab != nullptr) {
        FPlatformProcess::FreeDllHandle(this->_benchlab);
        this->_benchlab = nullptr;
    }
    if (this->_nvapi != nullptr) {
        FPlatformProcess::FreeDllHandle(this->_nvapi);
        this->_nvapi = nullptr;
    }
    if (this->_powenetics != nullptr) {
        FPlatformProcess::FreeDllHandle(this->_powenetics);
        this->_powenetics = nullptr;
    }
    if (this->_pwrowg != nullptr) {
        FPlatformProcess::FreeDllHandle(this->_pwrowg);
        this->_pwrowg = nullptr;
    }
}


/*
 * FUnrealPowerOverwhelmingModule::StartupModule
 */
void FUnrealPowerOverwhelmingModule::StartupModule(void) {
    auto& plugins = IPluginManager::Get();
    auto plugin = plugins.FindPlugin(TEXT("UnrealPowerOverwhelming"));
    check(plugin != nullptr);

    if (plugin != nullptr) {
        {
            auto path = FPaths::Combine(
                plugin->GetBaseDir(),
                TEXT("Binaries"),
                FPlatformProcess::GetBinariesSubdirectory(),
                TEXT("libbenchlab.dll"));
            UE_LOG(LogPwrOwg, Verbose, TEXT("Preparing delayed loaded power ")
                TEXT("libary \"%s\"."), *path);
            this->_benchlab = FPlatformProcess::GetDllHandle(*path);
            if (this->_benchlab == nullptr) {
                UE_LOG(LogPwrOwg, Error, TEXT("Failed to load power library ")
                    TEXT("\"%s\"."), *path);
            }
        }
        {
            auto path = FPaths::Combine(
                plugin->GetBaseDir(),
                TEXT("Binaries"),
                FPlatformProcess::GetBinariesSubdirectory(),
                TEXT("pwrovrnvapi.dll"));
            UE_LOG(LogPwrOwg, Verbose, TEXT("Preparing delayed loaded power ")
                TEXT("libary \"%s\"."), *path);
            this->_nvapi = FPlatformProcess::GetDllHandle(*path);
            if (this->_nvapi == nullptr) {
                UE_LOG(LogPwrOwg, Error, TEXT("Failed to load power library ")
                    TEXT("\"%s\"."), *path);
            }
        }
        {
            auto path = FPaths::Combine(
                plugin->GetBaseDir(),
                TEXT("Binaries"),
                FPlatformProcess::GetBinariesSubdirectory(),
                TEXT("libpowenetics.dll"));
            UE_LOG(LogPwrOwg, Verbose, TEXT("Preparing delayed loaded power ")
                TEXT("libary \"%s\"."), *path);
            this->_powenetics = FPlatformProcess::GetDllHandle(*path);
            if (this->_powenetics == nullptr) {
                UE_LOG(LogPwrOwg, Error, TEXT("Failed to load power library ")
                    TEXT("\"%s\"."), *path);
            }
        }
        {
            auto path = FPaths::Combine(
                plugin->GetBaseDir(),
                TEXT("Binaries"),
                FPlatformProcess::GetBinariesSubdirectory(),
                TEXT("power_overwhelming.dll"));
            UE_LOG(LogPwrOwg, Verbose, TEXT("Preparing delayed loaded power ")
                TEXT("libary \"%s\"."), *path);
            this->_pwrowg = FPlatformProcess::GetDllHandle(*path);
            if (this->_pwrowg == nullptr) {
                UE_LOG(LogPwrOwg, Error, TEXT("Failed to load power library ")
                    TEXT("\"%s\"."), *path);
            }
        }

        // The stuff above works for the editor, but not for the packaged game.
        // Hacking the PATH variable seems to be a functional workaround ...
        {
            auto path = FPlatformMisc::GetEnvironmentVariable(TEXT("PATH"));
            path += FPlatformMisc::GetPathVarDelimiter();
            path += FPaths::Combine(
                plugin->GetBaseDir(),
                TEXT("Binaries"),
                FPlatformProcess::GetBinariesSubdirectory());
            UE_LOG(LogPwrOwg, Verbose, TEXT("Updating path variable to \"%s\"."),
                *path);
            FPlatformMisc::SetEnvironmentVar(TEXT("PATH"), *path);
        }
    }
}

#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(FUnrealPowerOverwhelmingModule, UnrealPowerOverwhelming)


DEFINE_LOG_CATEGORY(LogPwrOwg)
