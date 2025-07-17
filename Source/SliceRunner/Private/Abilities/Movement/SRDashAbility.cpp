// Copyright (c) 2025 algorithmicbird. All rights reserved.

#include "Abilities/Movement/SRDashAbility.h"
#include "PlayerCharacter/SRBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void USRDashAbility::StartAbility(AActor *Instigator, const FSRAbilityActivationContext &Context)
{
    Super::StartAbility(Instigator, Context);
    Character = CastChecked<ASRBaseCharacter>(Instigator);
    if (Character->GetCharacterMovement()->IsFalling())
    {
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
    }
}

void USRDashAbility::StopAbility(AActor *Instigator)
{
    Super::StopAbility(Instigator);
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
    const FVector DashDir = Character->Controller->GetControlRotation().Vector();
    const float DashStrength = 1000.0f;
    Character->LaunchCharacter(DashDir * DashStrength, true, true);
}
