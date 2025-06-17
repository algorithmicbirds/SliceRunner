// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "Components/Raycaster/SRRaycastSensor.h"

#include "Engine/Engine.h"
void USRRaycastSensor::Cast()
{
    FVector EndPosition = Origin + CastDirection * CastLength;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());

    bool rayHit = GetWorld()->LineTraceSingleByChannel(
        hitInfo, Origin, EndPosition, ECC_GameTraceChannel1, Params, FCollisionResponseParams()
    );
}

void USRRaycastSensor::DebugCast()
{
    Cast();
    FVector EndPosition = Origin + CastDirection * CastLength;

    FColor LineColor = hitInfo.bBlockingHit ? FColor::Green : FColor::Red;

    DrawDebugLine(GetWorld(), Origin, EndPosition, LineColor, false, 1.0f, 0, 2.0f);

    if (hitInfo.bBlockingHit)
    {
        DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 10.0f, 18, FColor::Yellow, false, 6.0f);

        GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::MakeRandomColor(), TEXT("Hit Detected"));
    }
}
