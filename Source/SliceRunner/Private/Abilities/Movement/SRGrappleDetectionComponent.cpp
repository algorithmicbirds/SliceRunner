// Copyright (c) 2025 algorithmicbird. All rights reserved.

#include "Abilities/Movement/SRGrappleDetectionComponent.h"
#include "World/GrapplePoint/SRGrapplePoint.h"
#include "PlayerCharacter/SRBaseCharacter.h"

USRGrappleDetectionComponent::USRGrappleDetectionComponent() { PrimaryComponentTick.bCanEverTick = false; }

void USRGrappleDetectionComponent::BeginPlay()
{
    Super::BeginPlay();
    Character = CastChecked<ASRBaseCharacter>(GetOwner());
}

FHitResult USRGrappleDetectionComponent::GetFirstValidGrapplePoint()
{
    if (!Character)
        return FHitResult();

    const float TraceRadius = 50.0f;
    const float TraceDistance = 1200.0f;
    const FVector Start = Character->GetActorLocation();
    const FVector End = Start + Character->GetControlRotation().Vector() * TraceDistance;

    TArray<FHitResult> OutHits;
    FCollisionQueryParams QueryParam;
    QueryParam.AddIgnoredActor(Character->GetOwner());

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);

    bool bHit = GetWorld()->SweepMultiByObjectType(
        OutHits, Start, End, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(TraceRadius), QueryParam
    );

    for (const FHitResult &Hit : OutHits)
    {
        if (Cast<ASRGrapplePoint>(Hit.GetActor()))
        {
            return Hit;
        }
    }

    return FHitResult();
}

FHitResult USRGrappleDetectionComponent::CheckForGrapplePoints()
{
    if (GrapplePoint)
    {
        GrapplePoint->SetGrappleIconVisible(false);
        GrapplePoint = nullptr;
    }

    FHitResult Hit = GetFirstValidGrapplePoint();

    if (ASRGrapplePoint *HitPoint = Cast<ASRGrapplePoint>(Hit.GetActor()))
    {
        HitPoint->SetGrappleIconVisible(true);
        GrapplePoint = HitPoint;
    }

    return Hit;
}

void USRGrappleDetectionComponent::EnableGrappleDetection()
{
    GetWorld()->GetTimerManager().SetTimer(
        GrappleCheckTimerHandle, FTimerDelegate::CreateLambda([this]() { CheckForGrapplePoints(); }), 0.1f, true
    );
}

void USRGrappleDetectionComponent::DisableGrappleDetection()
{
    GetWorld()->GetTimerManager().ClearTimer(GrappleCheckTimerHandle);
}