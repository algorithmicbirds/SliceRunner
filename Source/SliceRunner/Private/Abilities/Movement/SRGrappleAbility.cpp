// Copyright (c) 2025 algorithmicbird. All rights reserved.

#include "Abilities/Movement/SRGrappleAbility.h"
#include "PlayerCharacter/SRBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Core/SRAbilityActivationContext.h"
#include "Debug/DebugHelper.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

USRGrappleAbility::USRGrappleAbility() {}

void USRGrappleAbility::StartAbility(AActor *Instigator, const FSRAbilityActivationContext &Context)
{
    Super::StartAbility(Instigator, Context);
    Character = CastChecked<ASRBaseCharacter>(Instigator);
    FHitResult Results = Context.HitResults;
    Debug::Print("Start Ability: Grapple");
    Grapple(Results);
}

void USRGrappleAbility::StopAbility(AActor *Instigator)
{
    Super::StopAbility(Instigator);
    ResetGrappleState();
}

void USRGrappleAbility::Grapple(const FHitResult &HitResult)
{
    if (!HitResult.bBlockingHit || !Character)
        return;

    bIsGrappling = true;
    CurrentGrappleTarget = HitResult.ImpactPoint;

    Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    Character->GetCharacterMovement()->StopMovementImmediately();
    Character->GetCharacterMovement()->GravityScale = 0.0f;

    SpawnBeamEffect(CurrentGrappleTarget);
    GetWorld()->GetTimerManager().SetTimer(
        UpdateGrappleMovementTimerHandle, this, &USRGrappleAbility::UpdateGrappleMovement, 0.06f, true
    );
}

void USRGrappleAbility::UpdateGrappleMovement()
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

        if (ActiveBeamFX)
        {
            ActiveBeamFX->SetVectorParameter("EndPoint", CurrentGrappleTarget);
        }
    }
}

void USRGrappleAbility::ResetGrappleState()
{
    if (!Character)
        return;

    bIsGrappling = false;

    Character->GetCharacterMovement()->GravityScale = 1.0f;
    Character->GetCharacterMovement()->SetMovementMode(MOVE_Falling);

    DestroyBeamEffect();
    GetWorld()->GetTimerManager().ClearTimer(UpdateGrappleMovementTimerHandle);
}

void USRGrappleAbility::SpawnBeamEffect(const FVector &Target)
{
    if (!GrappleBeamFX || !Character)
        return;

    FVector Start = Character->GetMesh()->GetSocketLocation(GrappleStartSocketName);
    ActiveBeamFX = UNiagaraFunctionLibrary::SpawnSystemAttached(
        GrappleBeamFX,
        Character->GetMesh(),
        GrappleStartSocketName,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::SnapToTarget,
        true
    );

    if (ActiveBeamFX)
    {
        ActiveBeamFX->SetVectorParameter("EndPoint", Target);
    }
}

void USRGrappleAbility::DestroyBeamEffect()
{
    if (ActiveBeamFX)
    {
        ActiveBeamFX->Deactivate();
        ActiveBeamFX = nullptr;
    }
}
