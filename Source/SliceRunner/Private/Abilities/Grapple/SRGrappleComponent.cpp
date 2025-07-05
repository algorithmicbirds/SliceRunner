// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "Abilities/Grapple/SRGrappleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/GrapplePoint/SRGrapplePoint.h"
#include "CableComponent.h"

// Sets default values for this component's properties
USRGrappleComponent::USRGrappleComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
    // features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
    CableComponent->SetVisibility(false);
    // ...
}

// Called when the game starts
void USRGrappleComponent::BeginPlay()
{
    Super::BeginPlay();
    Character = Cast<ACharacter>(GetOwner());
}

// Called every frame
void USRGrappleComponent::TickComponent(
    float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsGrappling)
    {
        UpdateGrappleMovement();
    }
}

FHitResult USRGrappleComponent::CheckForGrapplePoints()
{
    if (!bIsGrappleAllowed || !Character)
        return FHitResult();

    float TraceRadius = 50.0f;
    FVector Start = Character->GetActorLocation();
    float TraceDistance = 1200.0f;
    FVector End = Start + Character->GetControlRotation().Vector() * TraceDistance;

    TArray<FHitResult> OutHits;
    FCollisionQueryParams QueryParam;
    QueryParam.AddIgnoredActor(GetOwner());

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel2);

    bool bHit = GetWorld()->SweepMultiByObjectType(
        OutHits, Start, End, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(TraceRadius), QueryParam
    );

    // Hide previous grapple icon
    if (GrapplePoint)
    {
        GrapplePoint->SetGrappleIconVisible(false);
        GrapplePoint = nullptr;
    }

    for (const FHitResult &Hit : OutHits)
    {
        ASRGrapplePoint *HitPoint = Cast<ASRGrapplePoint>(Hit.GetActor());
        if (HitPoint)
        {
            HitPoint->SetGrappleIconVisible(true);
            GrapplePoint = HitPoint;

            return Hit;
        }
    }

    return FHitResult();
}

void USRGrappleComponent::Grapple(const FHitResult &HitResult)
{
    if (!HitResult.bBlockingHit || !Character)
        return;

    if (AActor *HitActor = HitResult.GetActor())
    {
        CableComponent->SetAttachEndTo(HitActor, NAME_None);
        CableComponent->EndLocation = FVector::ZeroVector;
    }

    bIsGrappling = true;
    CurrentGrappleTarget = HitResult.ImpactPoint;

    Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    Character->GetCharacterMovement()->StopMovementImmediately();
    Character->GetCharacterMovement()->GravityScale = 0.0f;

    CableComponent->SetVisibility(true);
}

void USRGrappleComponent::UpdateGrappleMovement()
{
    if (!Character)
        return;
    FVector ToTarget = CurrentGrappleTarget - Character->GetActorLocation();
    float Distance = ToTarget.Size();

    if (Distance < 300.f)
    {
        ResetGrappleState();
    }
    else
    {
        FVector Direction = ToTarget.GetSafeNormal();
        Character->GetCharacterMovement()->Velocity = Direction * GrappleSpeed;
    }
}

void USRGrappleComponent::ResetGrappleState()
{
    if (!Character)
        return;
    bIsGrappling = false;
    CableComponent->SetVisibility(false);
    CableComponent->SetAttachEndTo(nullptr, NAME_None);

    Character->GetCharacterMovement()->GravityScale = 1.0f;
    Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}

void USRGrappleComponent::UpdateGrappleCheckTimer()
{
    if (bIsGrappleAllowed)
    {
        GetWorld()->GetTimerManager().SetTimer(
            GrappleCheckTimerHandle, FTimerDelegate::CreateLambda([this]() { CheckForGrapplePoints(); }), 0.1f, true
        );
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(GrappleCheckTimerHandle);
    }
}

void USRGrappleComponent::AttachCableToSocket(class USceneComponent *InParent, FName InSocketName)
{
    if (!CableComponent || !Character)
        return;
    CableComponent->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, InSocketName);
}
