// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "Components/FloatingComponent.h"
#include "Debug/DebugHelper.h"
#include "Math/UnrealMathUtility.h"

AFloatingComponent::AFloatingComponent() { PrimaryActorTick.bCanEverTick = true; }

void AFloatingComponent::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
    ElapsedTime = 0.0f;
}

void AFloatingComponent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ElapsedTime += DeltaTime;

    const float Amplitude = 100.0f;
    const float Frequency = 0.3f;

    FVector Offset = FVector(0, 0, FMath::Sin(ElapsedTime * Frequency * 2 * PI) * Amplitude);
    SetActorLocation(StartLocation + Offset);
}
