// Copyright Pagan Games. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "StableBuoyancyComponent.generated.h"

/**
 *
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STABLEBUOYANCY_API UStableBuoyancyComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStableBuoyancyComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "StableBuoyancy", meta = (ToolTip = "Number of virtual pontoons created alogn the waterline"))
	int NumPoints = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StableBuoyancy", meta = (ToolTip = "Buoyancy value", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float Buoyancy = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StableBuoyancy", meta = (Tooltip = "Drag Coefficient, proportional to mass. Use with caution. (Best values 0 - 0.5)"))
	float DragCoefficient = 0.1f;

protected:
	// Called when the gaame starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "StableBuoyancy")
	float GetWaterLevel(FVector WorldPosition) const;

	virtual float GetWaterLevel_Implementation(FVector WorldPosition) const;

private:
	void GeneratePantoons();
	TArray<FVector> Pontoons;
	void ApplyForceToPoint(UPrimitiveComponent *Root, FVector Point, float BaseForce, float DeltaTime);
};
