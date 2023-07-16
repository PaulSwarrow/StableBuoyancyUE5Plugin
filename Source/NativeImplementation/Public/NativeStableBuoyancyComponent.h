// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StableBuoyancyComponent.h"
#include "WaterBodyComponent.h"
#include "NativeStableBuoyancyComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NATIVEIMPLEMENTATION_API UNativeStableBuoyancyComponent : public UStableBuoyancyComponent
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StableBuoyancy", meta = (ToolTip = "Water body detection distance. Should be better that waves max height"))
	float WaterBodyDetectionDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StableBuoyancy", meta = (ToolTip = "Water body collision profile name."))
	FName WaterBodyCollisionProfileName = "WaterBodyCollision";

	virtual FWaterLevelInfo GetWaterLevel_Implementation(FVector WorldPosition) const override;
	
};
