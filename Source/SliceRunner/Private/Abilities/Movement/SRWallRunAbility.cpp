// Copyright (c) 2025 algorithmicbird. All rights reserved.


#include "Abilities/Movement/SRWallRunAbility.h"
#include "PlayerCharacter/SRBaseCharacter.h"
#include "Abilities/Core/SRAbilityActivationContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Debug/DebugHelper.h"

// Sets default values for this component's properties
USRWallRunAbility::USRWallRunAbility() {}

bool USRWallRunAbility::CheckForWall(const FHitResult &Hit)
{
    if (!Character)
    {
        return false;
    }
    float CharacterAndWallAlignment = FMath::Abs(FVector::DotProduct(Hit.ImpactNormal, Character->GetActorRightVector()));
    return CharacterAndWallAlignment > 0.7f;
}


void USRWallRunAbility::StartAbility(AActor *Instigator, const FSRAbilityActivationContext &Context)
{
    Super::StartAbility(Instigator, Context);
    Character = CastChecked<ASRBaseCharacter>(Instigator);
    if (CheckForWall(Context.HitResults))
    {
        StartWallRun(Context.HitResults);
    }
}

void USRWallRunAbility::StopAbility(AActor *Instigator)
{
    Super::StopAbility(Instigator);
    StopWallRun();
}

void USRWallRunAbility::StartWallRun(const FHitResult &Hit)
{
    if (!Character || bIsWallRunning)
    {
        return;
    }
    bIsWallRunning = true;
    if (Character->GetCharacterMovement()->IsFalling())
    {
        Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
        Character->GetCharacterMovement()->SetPlaneConstraintEnabled(true);
        Character->GetCharacterMovement()->SetPlaneConstraintNormal(Hit.ImpactNormal);
    }

    GetWorld()->GetTimerManager().SetTimer(
        WallRunCheckTimerHandle, this, &USRWallRunAbility::EvaluateWallRunStateWithWallChecks, 0.1f, true
    );
    GetWorld()->GetTimerManager().SetTimer(
        WallRunMovementCheckHandle, this, &USRWallRunAbility::CheckWallRunEndDueToNoMovement, 0.1f, true
    );
}

void USRWallRunAbility::StopWallRun()
{
    if (!Character)
    {
        return;
    }
    bIsWallRunning = false;
    Character->GetCharacterMovement()->SetPlaneConstraintEnabled(false);
    Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    GetWorld()->GetTimerManager().ClearTimer(WallRunCheckTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(WallRunMovementCheckHandle);
}

void USRWallRunAbility::EvaluateWallRunStateWithWallChecks()
{
    if (!bIsWallRunning || !Character)
    {
        return;
    }
    
    float CastLength = 100.0f;
    FVector CharacterRightVector = Character->GetActorRightVector();
    
    bool bIsRightClear = !PerformRayCast(CharacterRightVector, CastLength);
    bool bIsLeftClear = !PerformRayCast(-CharacterRightVector, CastLength);

    if (bIsRightClear && bIsLeftClear && bIsWallRunning)
    {
        StopWallRun();
    }
}

void USRWallRunAbility::CheckWallRunEndDueToNoMovement()
{
    if (!bIsWallRunning || !Character)
    {
        return;
    }

    if (Character->GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero())
    {
        StopWallRun();
    }
}

bool USRWallRunAbility::PerformRayCast(FVector CastDirection, float CastLength)
{
    FHitResult HitInfo;
    if (!Character)
    {
        return false;
    }
    FVector Origin = Character->GetActorLocation();
    FVector EndPosition = Origin + CastDirection * CastLength;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Character);

    bool rayHit = GetWorld()->LineTraceSingleByChannel(
        HitInfo, Origin, EndPosition, ECC_GameTraceChannel1, Params, FCollisionResponseParams()
    );
    return rayHit;
}
