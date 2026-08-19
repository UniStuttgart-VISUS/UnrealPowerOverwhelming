// <copyright file="UnrealPowerOverwhelming.Build.cs" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

using Microsoft.Extensions.Logging;
using System.Diagnostics;
using System.IO;
using UnrealBuildTool;


public class UnrealPowerOverwhelming : ModuleRules {

    public UnrealPowerOverwhelming(ReadOnlyTargetRules target) : base(target) {
        this.PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        this.bEnableExceptions = true;

        this.PublicDependencyModuleNames.AddRange(["Core"]);
        this.PrivateDependencyModuleNames.AddRange([
            "CoreUObject",
            "Engine",
            "Projects",
            "Slate",
            "SlateCore",
            "PowerOverwhelming"
        ]);

        // Tell the engine where the Power Overwhelming DLLs are located and
        // that these should not be loaded directly, but will be explicitly
        // loaded during plugin initialisation.
        var binaryPath = Path.GetFullPath(Path.Combine(
            this.PluginDirectory,
            "Binaries",
            this.Target.Platform.ToString()));
        foreach (var b in Directory.GetFiles(binaryPath, "*.dll")) {
            this.Logger.LogInformation("Adding {Path} as runtime dependency.", b);
            this.RuntimeDependencies.Add(b);
        }

        //this.PublicIncludePaths.AddRange([]);
        //this.PrivateIncludePaths.AddRange([]);
        //this.DynamicallyLoadedModuleNames.AddRange([]);
    }
}
