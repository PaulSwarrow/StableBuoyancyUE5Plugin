// Copyright Epic Games, Inc. All Rights Reserved.

#include "StableBuoyancy.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FStableBuoyancyModule"

void FStableBuoyancyModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
    
	auto& ModuleManager = FModuleManager::Get();
	const FName WaterModuleName("Water");
	const FName NativeImplementationModuleName(TEXT("NativeImplementation"));

	if (ModuleManager.IsModuleLoaded("Water"))
    {
        ModuleManager.LoadModuleChecked<IModuleInterface>("NativeImplementation");
    }
    else
    {
        FModuleManager::Get().OnModulesChanged().AddLambda([](FName Name, EModuleChangeReason Reason)
        {
            if (Name == "Water" && Reason == EModuleChangeReason::ModuleLoaded)
            {
                FModuleManager::Get().LoadModuleChecked<IModuleInterface>("NativeImplementation");
            }
            else if (Name == "Water" && Reason == EModuleChangeReason::ModuleUnloaded)
            {
                FModuleManager::Get().UnloadModule("NativeImplementation");
            }
        });
    }
}

void FStableBuoyancyModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStableBuoyancyModule, StableBuoyancy)