// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealPowerOverwhelming.h"

#define LOCTEXT_NAMESPACE "FUnrealPowerOverwhelmingModule"

void FUnrealPowerOverwhelmingModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FUnrealPowerOverwhelmingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealPowerOverwhelmingModule, UnrealPowerOverwhelming)