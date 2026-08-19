// <copyright file="UnrealPowerOverwhelming.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include "Modules/ModuleManager.h"


/// <summary>
/// The module class for the UnrealPowerOverwhelming plugin.
/// </summary>
class FUnrealPowerOverwhelmingModule : public IModuleInterface {

public:

    FUnrealPowerOverwhelmingModule(void) noexcept;

    virtual void ShutdownModule(void) override;

    virtual void StartupModule(void) override;

private:

    void *_nvapi;
    void *_pwrowg;
};
