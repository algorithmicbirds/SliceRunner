// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/BaseInstance/SRBaseAnimInstance.h"
#include "SRCharacterAnimInstance.generated.h"

class ASRBaseCharacter;
class UCharacterMovementComponent;
    /**
 *
 */
UCLASS()
class SLICERUNNER_API USRCharacterAnimInstance : public USRBaseAnimInstance
{
    GENERATED_BODY()
  public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

  protected:
    UPROPERTY()
    ASRBaseCharacter *Character;
    UPROPERTY()
    UCharacterMovementComponent *CharacterMovementComponent;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Locomotion")
    float GroundSpeed;
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Locomotion")
    bool bHasAcceleration;
};
