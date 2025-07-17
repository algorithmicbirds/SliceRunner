// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRGrappleDetectionComponent.generated.h"

class ASRGrapplePoint;
class ASRBaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USRGrappleDetectionComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USRGrappleDetectionComponent();

  protected:
    virtual void BeginPlay() override;

    FHitResult GetFirstValidGrapplePoint();
    void CheckForGrapplePoints();
    
    UPROPERTY()
    TObjectPtr<ASRGrapplePoint> GrapplePoint = nullptr;
    
    UPROPERTY()
    TObjectPtr<ASRBaseCharacter> Character = nullptr;
    FTimerHandle GrappleCheckTimerHandle;
    FHitResult LastGrappleHitResult;

  public:
    FHitResult GetGrappleHitResult();
    void EnableGrappleDetection();
    void DisableGrappleDetection();
};
