// Copyright 2023 W³odzimierz Iwanowski. All Rights Reserved.

#include "DeeplPlugin.h"

#define LOCTEXT_NAMESPACE "FDeeplPluginModule"

void FDeeplPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDeeplPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDeeplPluginModule, DeeplPlugin)