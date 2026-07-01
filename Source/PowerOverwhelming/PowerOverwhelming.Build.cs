// <copyright file="PowerOverwhelming.Build.cs" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

using Microsoft.Extensions.Logging;
using EpicGames.Core;
using UnrealBuildTool;
using System.IO;
using System.Text;
using System.Diagnostics;
using System;


public class PowerOverwhelming : ModuleRules {

    public PowerOverwhelming(ReadOnlyTargetRules target) : base(target) {
        this.Type = ModuleType.External;

        this.Logger.LogInformation("Configure CMake in {Folder}.",
            this.BuildFolder);
        var configure = this.ExecuteCommand("cmake",
            "-G", $"\"{(this.IsWindows ? "Visual Studio 17 2022" : "Unix Makefiles")}\"",
            "-S", $"\"{this.SourceFolder}\"",
            "-B", $"\"{this.BuildFolder}\"",
            $"-DCMAKE_INSTALL_PREFIX={this.InstallFolder}",
            $"-DPWROWG_BuildDemo=OFF",
            $"-DPWROWG_BuildDriver=OFF",
            $"-DPWROWG_BuildDumpReadings=OFF",
            $"-DPWROWG_BuildDumpSensors=OFF",
            $"-DPWROWG_BuildRtxConfig=OFF",
            $"-DPWROWG_BuildStablePower=OFF",
            $"-DPWROWG_BuildTests=OFF",
            $"-DPWROWG_BuildWeb=OFF",
            $"-D{this.GetFeatureState("PWROWG_WithAdl")}",
            $"-D{this.GetFeatureState("PWROWG_WithBenchlab")}",
            $"-D{this.GetFeatureState("PWROWG_WithIgcl")}",
            $"-D{this.GetFeatureState("PWROWG_WithNiDaqMx")}",
            $"-D{this.GetFeatureState("PWROWG_WithNvapi")}",
            $"-D{this.GetFeatureState("PWROWG_WithNvml")}",
            $"-D{this.GetFeatureState("PWROWG_WithPowenetics")}",
            $"-D{this.GetFeatureState("PWROWG_WithVisa")}");
        this.Logger.LogInformation("Build using CMake.");
        var build = this.ExecuteCommand("cmake",
            "--build", $"\"{this.BuildFolder}\"",
            "--config", $"\"{this.Configuration}\"",
            "--target", "\"install\"",
            "-j", Environment.ProcessorCount.ToString());

        this.ExternalDependencies.Add(this.SourceFolder);

        this.PublicIncludePaths.Add(Path.Combine(
            this.InstallFolder,
            "include"));

        this.PublicAdditionalLibraries.Add(Path.Combine(
            this.InstallFolder,
            "lib",
            "power_overwhelming.lib"));

        // TODO: figure out how to transport the public defines

        //this.DynamicallyLoadedModuleNames.AddRange(Directory.GetFiles(
        //    Path.Combine(this.InstallFolder, "bin"), "*.dll"));
    }

    /// <summary>
    /// Synchronously executes the specified <paramref name="command"/> with
    /// the given <paramref name="arguments"/>.
    /// </summary>
    /// <param name="command"></param>
    /// <param name="arguments"></param>
    /// <returns></returns>
    private int ExecuteCommand(string command, params string[] arguments) {
        var args = new StringBuilder();
        var cmd = (string) null;

        if (this.IsWindows) {
            cmd = "cmd.exe";
            args.Append("/c ");
        } else {
            cmd = "/bin/bash";
            args.Append("-c \"");
        }

        args.Append(command);

        if (arguments != null) {
            foreach (var a in arguments) {
                args.Append(' ');

                if (this.IsWindows) {
                    args.Append(a);
                } else {
                    args.Append(a.Replace("\"", "\\\""));
                }
            }
        }

        var pi = new ProcessStartInfo(cmd, args.ToString()) {
            CreateNoWindow = true,
            UseShellExecute = false,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            WorkingDirectory = Path.GetFullPath(this.ModuleDirectory)
        };

        this.Logger.LogInformation("Executing {Command} {Arguments}.",
            pi.FileName, pi.Arguments);

        var error = new StringBuilder();
        var output = new StringBuilder();
        var proc = Process.Start(pi);
        proc.OutputDataReceived += (_, e) => {
            if (e.Data is not null) {
                output.AppendLine(e.Data);
            }
        };
        proc.ErrorDataReceived += (_, e) => {
            if (e.Data is not null) {
                error.AppendLine(e.Data);
            }
        };
        proc.BeginOutputReadLine();
        proc.BeginErrorReadLine();
        proc.WaitForExit();

        this.Logger.LogInformation(
            "{Command} {Arguments} exited with {ExitCode}: {Output}",
            pi.FileName, pi.Arguments, proc.ExitCode, output.ToString());
        if (proc.ExitCode != 0) {
            this.Logger.LogError("{Error}", error.ToString());
        }

        return proc.ExitCode;
    }

    /// <summary>
    /// Gets whether the specified <paramref name="feature"/> is enabled or
    /// disabled via an environment variable, or assumes it enabled.
    /// </summary>
    /// <param name="feature">The feature name in the form of the
    /// &quot;PWROWG_With*&quot; CMake variables.</param>
    /// <returns>The final feature string to be passed to CMake.</returns>
    private string GetFeatureState(string feature) {
        var state = Environment.GetEnvironmentVariable(feature);
        state = state switch {
            null => "ON",
            "" => "ON",
            "1" => "ON",
            "true" => "ON",
            "0" => "OFF",
            "false" => "OFF",
            _ => state,
        };

        return $"{feature}={state}";
    }

    /// <summary>
    /// Gets the location of the CMake build tree.
    /// </summary>
    private string BuildFolder => Path.Combine(
        this.RootFolder,
        "Intermediate",
        "Build",
        this.Target.Platform.ToString(),
        "CMake");

    /// <summary>
    /// Gets the CMake configuration to build.
    /// </summary>
    private string Configuration => this.Target.Configuration switch {
        UnrealTargetConfiguration.Debug => "Debug",
        UnrealTargetConfiguration.DebugGame => "Debug",
        _ => "Release",
    };

    /// <summary>
    /// Gets the binary folder where to put the CMake output.
    /// </summary>
    private string InstallFolder => Path.Combine(
        this.RootFolder,
        "Binaries",
        this.Target.Platform.ToString());

    /// <summary>
    /// Gets whether we are building for Windows.
    /// </summary>
    private bool IsWindows => (BuildHostPlatform.Current.Platform
        == UnrealTargetPlatform.Win64);

    /// <summary>
    /// Gets the root folder of the plugin.
    /// </summary>
    private string RootFolder => Path.GetFullPath(Path.Combine(
        this.ModuleDirectory, "..", ".."));

    /// <summary>
    /// Gets the folder where the source of Power Overwhelming is located.
    /// </summary>
    private string SourceFolder => Path.Combine(
        this.ModuleDirectory, "power-overwhelming");
}
