// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Core/SRAbilityBase.h"
#include "SRGrappleAbility.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class ASRBaseCharacter;
class ASRGrapplePoint;

UCLASS()
class SLICERUNNER_API USRGrappleAbility : public USRAbilityBase
{
    GENERATED_BODY()

  public:
    USRGrappleAbility();

    virtual void StartAbility(AActor *Instigator, const FSRAbilityActivationContext &Context) override;
    virtual void StopAbility(AActor *Instigator) override;

  protected:
    UPROPERTY(EditDefaultsOnly, Category = "Grapple")
    UNiagaraSystem *GrappleBeamFX;

    UPROPERTY(EditDefaultsOnly, Category = "Grapple")
    FName GrappleStartSocketName = TEXT("GrappleSocket");

    UPROPERTY(EditDefaultsOnly, Category = "Grapple")
    float GrappleSpeed = 2500.0f;

    FTimerHandle UpdateGrappleMovementTimerHandle;
    FVector CurrentGrappleTarget;
    

    UPROPERTY()
    TObjectPtr<ASRBaseCharacter> Character = nullptr;
 
    UPROPERTY()
    TObjectPtr<UNiagaraComponent> ActiveBeamFX = nullptr;

    void Grapple(const FHitResult &HitResult);
    void UpdateGrappleMovement();
    void ResetGrappleState();
    void SpawnBeamEffect(const FVector &Target);
    void DestroyBeamEffect();
};
