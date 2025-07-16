// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRGrappleComponent.generated.h"

class ASRGrapplePoint;
class ASRBaseCharacter;
class UCableComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLICERUNNER_API USRGrappleComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    // Sets default values for this component's properties
    USRGrappleComponent();

  protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grapple", meta = (AllowPrivateAccess = "true"))
    UCableComponent *CableComponent;

  public:
    // Called every frame
    virtual void
    TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
    void Grapple(const FHitResult &HitResult);
    void UpdateGrappleCheckTimer();
    void AttachCableToSocket(USceneComponent *InParent, FName SocketName);
    void ResetGrappleState();
    FHitResult CheckForGrapplePoints();
    bool IsGrappleAllowed() const { return bIsGrappleAllowed; };
    bool IsGrappling() const { return bIsGrappling; };
    bool SetAllowGrapple(bool IsGrappleAllowed) { return bIsGrappleAllowed = IsGrappleAllowed; }; 

  private:
    void UpdateGrappleMovement();
    bool bIsGrappleAllowed = false;
    bool bIsGrappling = false;
    ASRGrapplePoint *GrapplePoint = nullptr;
    FVector CurrentGrappleTarget;
    FTimerHandle GrappleCheckTimerHandle;
    UPROPERTY()
    ASRBaseCharacter *Character;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float GrappleSpeed = 2000.0f;
};
