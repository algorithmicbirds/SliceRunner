// Copyright (c) 2025 algorithmicbird. All rights reserved.


#include "AnimInstance/SharedInstance/SRCharacterAnimInstance.h"
#include "PlayerCharacter/SRBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"



void USRCharacterAnimInstance::NativeInitializeAnimation()
{
    Character = Cast<ASRBaseCharacter>(TryGetPawnOwner());
    if (Character)
    {
        CharacterMovementComponent = Character->GetCharacterMovement();
    }
}

void USRCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    if (!Character || !CharacterMovementComponent)
        return;
    GroundSpeed = Character->GetVelocity().Size2D();
    bHasAcceleration = CharacterMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.0f;
}
