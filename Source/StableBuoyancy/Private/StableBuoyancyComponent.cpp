// Copyright Pagan Games. All rights reserved.

#include "StableBuoyancyComponent.h"	
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UStableBuoyancyComponent::UStableBuoyancyComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UStableBuoyancyComponent::BeginPlay()
{
    Super::BeginPlay();
    GeneratePontoons();
}

FWaterLevelInfo UStableBuoyancyComponent::GetWaterLevel_Implementation(FVector WorldPosition) const
{
    FWaterLevelInfo Result;
    Result.HasWaterBody = true;
    Result.WaterLevel = 0.0f;
    return Result;
}

void UStableBuoyancyComponent::GeneratePontoons()
{

    float SplineLength = GetSplineLength();
    float Increment = SplineLength / static_cast<float>(NumPoints - 1);

    for (int32 i = 0; i < NumPoints; i++)
    {
        float Distance = Increment * static_cast<float>(i);
        FVector Point = GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
        Pontoons.Add(Point);
    }
}

void UStableBuoyancyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Get actor's physic body
    auto Root = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    if(Root == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Stable Buoyancy Error: Root component is not UPrimitiveComponent. Reminder: Root component has to be a Static Mesh"));
        return;
    }
    // Calculate the gravity force
    auto Gravity = GetWorld()->GetGravityZ();
    auto Mass = Root->GetMass();
    FVector GravityForce = FVector(0.0f, 0.0f, Gravity) * Mass;

    // Calculate the spring force based on the displacement
    float BuoyancyValue = Gravity * Mass / 2; // sprint force to mass correlation
    float Prediction = GravityForce.Z / (BuoyancyValue * DeltaTime);

    float PontoonForce = BuoyancyValue / Pontoons.Num();
    for (const FVector &Panton : Pontoons)
    {
        FVector Point = GetOwner()->GetTransform().TransformPosition(Panton);
        ApplyForceToPoint(Root, Point + FVector(0, 0, -Prediction), PontoonForce, DeltaTime);
    }
}

void UStableBuoyancyComponent::ApplyForceToPoint(UPrimitiveComponent *Root, FVector Point, float BaseForce, float DeltaTime)
{

    auto WaterInfo = GetWaterLevel(Point);
    if(!WaterInfo.HasWaterBody) 
    {
        return;
    }
    auto DeltaZ = WaterInfo.WaterLevel - Point.Z;
    if (DeltaZ > 0.0f)
    {
        FVector SpringForce = -FVector(0.0f, 0.0f, BaseForce) * DeltaZ * DeltaTime;

        // Calculate the damping force based on the actor's velocity
        FVector Velocity = Root->GetPhysicsLinearVelocityAtPoint(Point);
        float DampingFactor = abs(BaseForce * DragCoefficient); // Adjust the damping factor as needed
        FVector DampingForce = -DampingFactor * Velocity * DeltaTime;

        // Apply the combined force to the actor's root component
        Root->AddForceAtLocation(Buoyancy * SpringForce + DampingForce, Point);
    }
}