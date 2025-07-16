// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Core/SRAbilityBase.h"
#include "SRWallRunAbility.generated.h"

class ASRBaseCharacter;

UCLASS()
class SLICERUNNER_API USRWallRunAbility : public USRAbilityBase
{
    GENERATED_BODY()

  public:
    // Sets default values for this component's properties
    USRWallRunAbility();

  public:
    virtual void StartAbility_Implementation(AActor *Instigator, const FSRAbilityActivationContext &Context) override;
    void StartWallRun(const FHitResult &Hit);
    void StopWallRun();
    bool CheckForWall(const FHitResult &Hit);
    bool IsWallRunning() const { return bIsWallRunning; }

  private:
    void EvaluateWallRunStateWithWallChecks();
    void CheckWallRunEndDueToNoMovement();
    bool PerformRayCast(FVector CastDirection, float CastLength);
    bool bIsWallRunning = false;
    FTimerHandle WallRunCheckTimerHandle;
    FTimerHandle WallRunMovementCheckHandle;
    UPROPERTY()
    TObjectPtr<ASRBaseCharacter> Character = nullptr;
};
