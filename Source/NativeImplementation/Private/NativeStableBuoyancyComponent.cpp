// Fill out your copyright notice in the Description page of Project Settings.

#include "NativeStableBuoyancyComponent.h"
#include "WaterSubsystem.h"

FWaterLevelInfo UNativeStableBuoyancyComponent::GetWaterLevel_Implementation(FVector WorldPosition) const
{
    auto World = GetWorld();
    
    FHitResult HitResult;
    FVector EndPosition = WorldPosition + FVector(0, 0, -WaterBodyDetectionDistance);

    FWaterLevelInfo Result;

    if(World->LineTraceSingleByProfile(HitResult, WorldPosition, EndPosition, WaterBodyCollisionProfileName)) {
        auto Body = Cast<AWaterBody>(HitResult.GetActor());
        if(Body != nullptr) {
            auto WaterComponet = Body->GetWaterBodyComponent();
            Result.HasWaterBody = true;
            EWaterBodyQueryFlags QueryFlags =    
                EWaterBodyQueryFlags::ComputeLocation | 
                EWaterBodyQueryFlags::ComputeNormal | 
                EWaterBodyQueryFlags::ComputeImmersionDepth | 
                EWaterBodyQueryFlags::ComputeVelocity | 
                EWaterBodyQueryFlags::IncludeWaves;

            FWaterBodyQueryResult QueryResult = WaterComponet->QueryWaterInfoClosestToWorldLocation(WorldPosition, QueryFlags);
            Result.WaterLevel = QueryResult.GetWaterSurfaceLocation().Z;
        }
    }

    return Result;
}
