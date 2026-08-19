# Power Overwhelming for Unreal Engine
This plugin allows for integrating the [Power Overwhelming library](https://github.com/UniStuttgart-VISUS/power-overwhelming) in an Unreal project.

## Building
Check out the plugin including all submodules into the Plugins directory of your project and activate the plugin. The Unreal Build Tool should handle the rest.

You can set environments named like the feature switches of the Power Overwhelming CMake project (e.g. `PWROWG_WithNvapi`) to control sensor support in the Unreal build. For instance, when setting `PWROWG_WithNvapi=OFF`, the NVAPI-based sensor will not be available.

## Usage
Your Unreal project can interact with Power Overwhelming via the `Power` component of the plugin. You can add this component to one of your actors, or add one of the `ManualPowerActor` or `AutoPowerActor` actors from the plugin to the game. The former exposes functions for starting the measurement, whereas the latter will start sampling the sensors in `BeginPlay` and stop the sensors in `EndPlay`.
