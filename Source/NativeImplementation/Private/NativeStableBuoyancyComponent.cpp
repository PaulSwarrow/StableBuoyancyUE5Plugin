// Fill out your copyright notice in the Description page of Project Settings.

#include "NativeStableBuoyancyComponent.h"
#include "WaterSubsystem.h"

float UNativeStableBuoyancyComponent::GetWaterLevel_Implementation(FVector WorldPosition) const
{
    if(WaterBody == nullptr) {
        return 0.0f;
    }
    
    EWaterBodyQueryFlags QueryFlags =
        EWaterBodyQueryFlags::ComputeLocation | EWaterBodyQueryFlags::ComputeNormal | EWaterBodyQueryFlags::ComputeImmersionDepth | EWaterBodyQueryFlags::ComputeVelocity | EWaterBodyQueryFlags::IncludeWaves;

    FWaterBodyQueryResult QueryResult = WaterBody->QueryWaterInfoClosestToWorldLocation(WorldPosition, QueryFlags);

    return QueryResult.GetWaterSurfaceLocation().Z;
}
