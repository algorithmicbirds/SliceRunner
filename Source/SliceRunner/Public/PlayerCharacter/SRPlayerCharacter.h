// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "Interfaces/Gate/GateRuleInterface.h"
#include "PlayerCharacter/SRBaseCharacter.h"
#include "SRPlayerCharacter.generated.h"

class UCameraComponent;
class USRDataAsset_InputConfig;
class UCableComponent;
class USRWallRunComponent;
class USRGrappleComponent;
class UStaticMeshComponent;
class USRAbilityManager;

UCLASS()
class SLICERUNNER_API ASRPlayerCharacter : public ASRBaseCharacter
{
    GENERATED_BODY()

  public:
    ASRPlayerCharacter();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
    void SetGateAbilityFlags(const FGateAbilityFlags &InFlags);

  protected:
    virtual void BeginPlay() override;
    virtual void Landed(const FHitResult &Hit) override;
    virtual void NotifyHit(
        UPrimitiveComponent *MyComp,
        AActor *Other,
        UPrimitiveComponent *OtherComp,
        bool bSelfMoved,
        FVector HitLocation,
        FVector HitNormal,
        FVector NormalImpulse,
        const FHitResult &Hit
    ) override;

  private:
#pragma region Components

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent *FirstPersonMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent *FirstPersonCameraComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USRGrappleComponent *GrappleComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent *WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USRAbilityManager *AbilityManager;

#pragma endregion

#pragma region Inputs
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
    USRDataAsset_InputConfig *InputDataAssetConfig;

    void Input_Move(const FInputActionValue &InputActionValue);
    void Input_Look(const FInputActionValue &InputActionValue);
    void Input_Jump(const FInputActionValue &InputActionValue);
    void Input_Dash(const FInputActionValue &InputActionValue);
    void Input_Grapple(const FInputActionValue &InputActionValue);

#pragma endregion

#pragma region Dash
    void StartDashing();
    void StopDashing();
    bool bIsDashing = false;
#pragma endregion

  private:
    FGateAbilityFlags CurrentZoneFlags;

  public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float JumpVelocity = 600.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float AirControl = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float FallingBrakingDeceleration = 1500.0f;
};