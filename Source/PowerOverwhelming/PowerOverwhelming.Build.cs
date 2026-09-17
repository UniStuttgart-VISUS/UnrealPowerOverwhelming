// <copyright file="PowerOverwhelming.Build.cs" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

using Microsoft.Extensions.Logging;
using EpicGames.Core;
using UnrealBuildTool;
using System;
using System.Diagnostics;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;


/// <summary>
/// Describes how Unreal should build the Power Overwhelming library using
/// CMake.
/// </summary>
/// <remarks>
/// <para>The Power Overwhelming library is assumed to be located in the
/// &quot;power-overwhelming&quot; subfolder of this module. This can be
/// achieved by cloning the submodules when obtaining the plugin from our
/// GitHub repository.</para>
/// </remarks>
public class PowerOverwhelming : ModuleRules {

    public PowerOverwhelming(ReadOnlyTargetRules target) : base(target) {
        this.Type = ModuleType.External;

        var generator = "Unix Makefiles";
        if (this.IsWindows) {
            generator = target.WindowsPlatform.Compiler switch {
                WindowsCompiler.VisualStudio2022 => "Visual Studio 17 2022",
                // TODO: figure out why UE is reporting VS2026 when we use VS2022
                WindowsCompiler.VisualStudio2026 => "Visual Studio 17 2022",
                _ => throw new Exception("Unsupported compiler for CMake. Make "
                    + "sure to add new versions of Visual Studio in the Unreal "
                    + "build script.")
            };
        }
        this.Logger.LogInformation("Using CMake generator \"{Generator}\".",
            generator);

        var daqmx = this.DaqmxInclude;
        var visa = this.VisaInclude;

        this.Logger.LogInformation("Configure CMake in {Folder}.",
            this.BuildFolder);
        var configure = this.ExecuteCommand("cmake",
            "-G", $"\"{generator}\"",
            "-S", $"\"{this.SourceFolder}\"",
            "-B", $"\"{this.BuildFolder}\"",
            $"-DCMAKE_INSTALL_PREFIX={this.IntermediateFolder}",
            // The next line is important: the DLL must be located in the plugin
            // directory in order to be loaded by Unreal.
            $"-DCMAKE_INSTALL_BINDIR={this.BinaryFolder}",
            $"-DPWROWG_BuildDemo=OFF",
            $"-DPWROWG_BuildDriver=OFF",
            $"-DPWROWG_BuildDumpReadings=OFF",
            $"-DPWROWG_BuildDumpSensors=OFF",
            $"-DPWROWG_BuildRtxConfig=OFF",
            $"-DPWROWG_BuildStablePower=OFF",
            $"-DPWROWG_BuildTests=OFF",
            $"-DPWROWG_BuildWeb=OFF",
            $"-D{this.GetFeatureState("PWROWG_WithAdl")}",
            $"-D{this.GetFeatureState("PWROWG_WithBenchlab", "ON")}",
            $"-D{this.GetFeatureState("PWROWG_WithIgcl")}",
            $"-D{this.GetFeatureState("PWROWG_WithNiDaqMx", (daqmx is not null) ? "ON" : "OFF")}",
            $"-D{this.GetFeatureState("PWROWG_WithNvapi")}",
            $"-D{this.GetFeatureState("PWROWG_WithNvml")}",
            $"-DPWROWG_WithParquet=OFF",
            $"-D{this.GetFeatureState("PWROWG_WithPowenetics", "ON")}",
            $"-D{this.GetFeatureState("PWROWG_WithVisa", (visa is not null) ? "ON" : "OFF")}");
        this.Logger.LogInformation("Build using CMake.");
        var build = this.ExecuteCommand("cmake",
            "--build", $"\"{this.BuildFolder}\"",
            //"--config", $"\"{this.Configuration}\"",
            //"--config", "\"Release\"",
            "--config", "\"Debug\"",
            "--target", "\"install\"",
            "-j", Environment.ProcessorCount.ToString());

        this.ExternalDependencies.AddRange([
            this.SourceFolder
        ]);

        this.PublicIncludePaths.Add(Path.Combine(
            this.IntermediateFolder,
            "include"));
        //if (daqmx is not null) {
        //    this.PublicIncludePaths.Add(daqmx);
        //}
        if (visa is not null) {
            this.PublicIncludePaths.Add(visa);
        }

        this.PublicAdditionalLibraries.Add(Path.Combine(
            this.IntermediateFolder,
            "lib",
            "power_overwhelming.lib"));

        // Figure out what CMake has defined publicly when building the library.
        // We want to carry over these defines to enable some APIs that are
        // guarded by preprocessor macros.
        var cmakeConfig = Path.Combine(
            this.IntermediateFolder,
            "lib",
            "cmake",
            "power_overwhelming",
            "power_overwhelmingTargets.cmake");
        try {
            var rx = new Regex("INTERFACE_COMPILE_DEFINITIONS\\s+\"([^\"]+)\"");
            var txt = File.ReadAllText(cmakeConfig);
            if (rx.Match(txt) is Match m && m.Success) {
                var defines = m.Groups[1].Value.Split(';');
                foreach (var d in defines) {
                    this.PublicDefinitions.Add(d);
                }
            } else {
                this.Logger.LogWarning("Failed to find public defines in the "
                    + "CMake configuration in \"{Path}\".", cmakeConfig);
            }
        } catch (Exception ex) {
            this.Logger.LogError(ex, "Failed to obtain CMake configuration "
                + "from \"{Path}\".", cmakeConfig);
        }
    }

    #region Private properties
    /// <summary>
    /// Gets the binary folder where CMake should put the DLLs.
    /// </summary>
    private string BinaryFolder => Path.Combine(
        this.RootFolder,
        "Binaries",
        this.Target.Platform.ToString());

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
    /// Gets, if available, the folder where the NI-DAQmx includes are located.
    /// </summary>
    private string DaqmxInclude {
        get {
            var env = Environment.GetEnvironmentVariable("NIEXTCCOMPILERSUPP");

            if (string.IsNullOrWhiteSpace(env) && Directory.Exists(env)) {
                this.Logger.LogInformation("Found NI-DAQmx in \"{Path}\".", env);
                return env;
            }

            string[] installs = [
                Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
                Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
            ];

            foreach (var i in installs) {
                var p = Path.Combine(i,
                    "National Instruments",
                    "Shared",
                    "ExternalCompilerSupport",
                    "C");
                if (Directory.Exists(p)) {
                    this.Logger.LogInformation("Found NI-DAQmx in \"{Path}\".", p);
                    return Path.Combine(p, "include");
                }
            }

            return null;
        }
    }

    /// <summary>
    /// The intermediate folder where we will deploy the public headers, import
    /// libraries, and other intermediate files.
    /// </summary>
    private string IntermediateFolder => Path.Combine(
        this.RootFolder,
        "Intermediate",
        "Build",
        this.Target.Platform.ToString(),
        "PowerOverwhelming");

    /// <summary>
    /// Gets whether we are building for Windows.
    /// </summary>
    private bool IsWindows => (BuildHostPlatform.Current.Platform
        == UnrealTargetPlatform.Win64);

    /// <summary>
    /// Gets the root folder of the plugin.
    /// </summary>
    private string RootFolder => Path.GetFullPath(this.PluginDirectory);

    /// <summary>
    /// Gets the folder where the source of Power Overwhelming is located.
    /// </summary>
    private string SourceFolder => Path.Combine(
        this.ModuleDirectory, "power-overwhelming");

    /// <summary>
    /// Gets, if available, the folder where the VISA includes are located.
    /// </summary>
    private string VisaInclude {
        get {
            string[] installs = [
                Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
                Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86)
            ];
            string[] includes = [
                Path.Combine("WinNT", "RsVisa", "include"),
                Path.Combine("Win64", "RsVisa", "include"),
                Path.Combine("include", "rsvisa")
            ];

            foreach (var i in installs) {
                var p = Path.Combine(i, "IVI Foundation", "VISA");

                foreach (var j in includes) {
                    var q = Path.Combine(p, j);
                    if (Directory.Exists(q)) {
                        this.Logger.LogInformation("Found VISA in \"{Path}\".", q);
                        return q;
                    }
                }
            }

            return null;
        }
    }
    #endregion

    #region Private methods
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
    /// <param name="default">The default state used when the environment
    /// variable for the feature was not found.</param>
    /// <returns>The final feature string to be passed to CMake.</returns>
    private string GetFeatureState(string feature, string @default = "ON") {
        var state = Environment.GetEnvironmentVariable(feature);
        state = state switch {
            null => @default,
            "" => @default,
            "1" => "ON",
            "true" => "ON",
            "0" => "OFF",
            "false" => "OFF",
            _ => state,
        };

        return $"{feature}={state}";
    }
    #endregion
}
