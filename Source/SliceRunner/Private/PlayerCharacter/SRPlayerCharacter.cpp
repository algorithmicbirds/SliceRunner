// Copyright (c) 2025 algorithmicbird. All rights reserved.

#include "PlayerCharacter/SRPlayerCharacter.h"
#include "Core/Input/SRDataAsset_InputConfig.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTags/SRGameplayTags.h"
#include "Core/Input/SREnhancedInputComponent.h"
#include "Debug/DebugHelper.h"
#include "Abilities/Core/SRAbilityManager.h"
#include "Abilities/Core/SRAbilityActivationContext.h"
#include "Abilities/Movement/SRGrappleDetectionComponent.h"


ASRPlayerCharacter::ASRPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(32.0f, 96.0f);

    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    GetMesh()->SetupAttachment(FirstPersonCameraComponent);

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(GetMesh(), TEXT("katana_socket"));

    GetCharacterMovement()->BrakingDecelerationFalling = FallingBrakingDeceleration;
    GetCharacterMovement()->AirControl = AirControl;
    GetCharacterMovement()->JumpZVelocity = JumpVelocity;

    GrappleDetectionComponent = CreateDefaultSubobject<USRGrappleDetectionComponent>(TEXT("GrappleDetectionComponent"));

    AbilityManager = CreateDefaultSubobject<USRAbilityManager>(TEXT("AbilityManager"));
}



void ASRPlayerCharacter::BeginPlay() { 
    Super::BeginPlay(); 
}

void ASRPlayerCharacter::Landed(const FHitResult &Hit)
{
    Super::Landed(Hit);
}

void ASRPlayerCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ASRPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    checkf(InputDataAssetConfig, TEXT("Forgot to assign a valid DataAsset_InputConfig in the editor!"));

    ULocalPlayer *LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
    UEnhancedInputLocalPlayerSubsystem *Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
    check(Subsystem);
    Subsystem->AddMappingContext(InputDataAssetConfig->DefaultMappingContext, 0);

    USREnhancedInputComponent *SRInputComp = CastChecked<USREnhancedInputComponent>(PlayerInputComponent);

    SRInputComp->BindNativeInputAction(
        InputDataAssetConfig,
        SRGameplayTags::InputTag_Move,
        ETriggerEvent::Triggered,
        this,
        &ASRPlayerCharacter::Input_Move
    );

    SRInputComp->BindNativeInputAction(
        InputDataAssetConfig,
        SRGameplayTags::InputTag_Look,
        ETriggerEvent::Triggered,
        this,
        &ASRPlayerCharacter::Input_Look
    );

    SRInputComp->BindNativeInputAction(
        InputDataAssetConfig,
        SRGameplayTags::InputTag_Jump,
        ETriggerEvent::Triggered,
        this,
        &ASRPlayerCharacter::Input_Jump
    );

    SRInputComp->BindNativeInputAction(
        InputDataAssetConfig,
        SRGameplayTags::InputTag_Dash,
        ETriggerEvent::Triggered,
        this,
        &ASRPlayerCharacter::Input_Dash
    );

    SRInputComp->BindNativeInputAction(
        InputDataAssetConfig,
        SRGameplayTags::InputTag_Grapple,
        ETriggerEvent::Triggered,
        this,
        &ASRPlayerCharacter::Input_Grapple
    );
}

void ASRPlayerCharacter::SetGateAbilityFlags(const FGateAbilityFlags &InFlags)
{
    CurrentGateAbilityFlags = InFlags;
    FString DebugMsg = LexToString(InFlags.bCanGrapple);
    Debug::Print(DebugMsg);
    if (InFlags.bCanGrapple)
    {
        GrappleDetectionComponent->EnableGrappleDetection();
    }
    else
    {
        GrappleDetectionComponent->DisableGrappleDetection();
    }
}

#pragma region Inputs
void ASRPlayerCharacter::Input_Move(const FInputActionValue &InputActionValue)
{
    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
    const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

    if (MovementVector.Y != 0.0f)
    {
        const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
        AddMovementInput(ForwardDirection, MovementVector.Y);
    
    }

    if (MovementVector.X != 0.0f)
    {
       /* if (WallRunComponent->IsWallRunning())
        {
            WallRunComponent->StopWallRun();
        }*/
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
        if (bIsDashing)
        {
            const float MoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
            FVector DeltaMove = RightDirection * MovementVector.X * MoveSpeed * GetWorld()->GetDeltaSeconds();
            AddActorWorldOffset(DeltaMove, true);
        }
        else
        {
            AddMovementInput(RightDirection, MovementVector.X);
        }
    }
}

void ASRPlayerCharacter::Input_Look(const FInputActionValue &InputActionValue)
{
    const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
    if (LookAxisVector.X != 0.0f)
    {
        AddControllerYawInput(LookAxisVector.X);
    }

    if (LookAxisVector.Y != 0.0f)
    {
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ASRPlayerCharacter::Input_Jump(const FInputActionValue &InputActionValue)
{
    if (InputActionValue.Get<bool>())
    {
        //if (WallRunComponent->IsWallRunning())
        //{
        //    WallRunComponent->StopWallRun();
        //    Debug::Print("Jump DuringWallRun");
        //    FVector PlaneContraintNormal = GetCharacterMovement()->GetPlaneConstraintNormal();
        //    // launch character based on its speeed
        //    const float DashStrength = GetVelocity().Length();
        //    FVector UpwardForce = FVector(0, 0, DashStrength);
        //    LaunchCharacter(PlaneContraintNormal * DashStrength + UpwardForce, false, true);
        //}
        Jump();
    }
    else
    {
        StopJumping();
    }
}

void ASRPlayerCharacter::Input_Dash(const FInputActionValue &InputActionValue)
{
    if (InputActionValue.Get<bool>())
    {
        FSRAbilityActivationContext Context{};
        AbilityManager->StartAbilityByName(this, "Dash", Context);
    }
    else
    {
        AbilityManager->StopAbilityByName(this, "Dash");
    }
}

void ASRPlayerCharacter::Input_Grapple(const FInputActionValue &InputActionValue)
{
    if (InputActionValue.Get<bool>())
    {
        FSRAbilityActivationContext Context;
        Context.HitResults = GrappleDetectionComponent->GetGrappleHitResult();
        AbilityManager->StartAbilityByName(this, "Grapple", Context);
    }
    else
    {
        AbilityManager->StopAbilityByName(this, "Grapple");
    }
}

#pragma endregion

void ASRPlayerCharacter::NotifyHit(
    UPrimitiveComponent *MyComp,
    AActor *Other,
    UPrimitiveComponent *OtherComp,
    bool bSelfMoved,
    FVector HitLocation,
    FVector HitNormal,
    FVector NormalImpulse,
    const FHitResult &Hit
)
{
    FSRAbilityActivationContext Context;
    Context.HitResults = Hit;
    AbilityManager->StartAbilityByName(this, "WallRun", Context);
}
