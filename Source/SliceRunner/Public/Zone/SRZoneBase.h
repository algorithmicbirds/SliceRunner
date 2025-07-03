// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Zones/ZoneRuleInterface.h"
#include "SRZoneBase.generated.h"

class UBoxComponent;

UCLASS()
class SLICERUNNER_API ASRZoneBase : public AActor, public IZoneRuleInterface
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    ASRZoneBase();
    UPROPERTY(EditAnywhere)
    FZoneAbilityFlags AbilityFlags;

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    UPROPERTY(VisibleAnywhere)
    UBoxComponent *ZoneTrigger;
  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    virtual const FZoneAbilityFlags &GetZoneAbilities() const override;

    private:
    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent *OverlappedComp,
        AActor *OtherActor,
        UPrimitiveComponent *OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult &SweepResult
    );
};
