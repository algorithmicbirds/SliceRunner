// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "PlayerCharacter/SRPlayerCharacter.h"
#include "Input/SRDataAsset_InputConfig.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameplayTags/SRGameplayTags.h"
#include "Input/SREnhancedInputComponent.h"
#include "Debug/DebugHelper.h"
#include "UI/GrapplePoint/SRGrapplePoint.h"
#include "Abilities/WallRun/SRWallRunComponent.h"
#include "Abilities/Grapple/SRGrappleComponent.h"
#include "Abilities/Core/SRAbilityManager.h"


ASRPlayerCharacter::ASRPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(32.0f, 96.0f);

    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
    FirstPersonMesh->SetOnlyOwnerSee(true);
    FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FirstPersonMesh->SetCastShadow(false);


    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(FirstPersonMesh, TEXT("katana_socket"));

    GetCharacterMovement()->BrakingDecelerationFalling = FallingBrakingDeceleration;
    GetCharacterMovement()->AirControl = AirControl;
    GetCharacterMovement()->JumpZVelocity = JumpVelocity;

    WallRunComponent = CreateDefaultSubobject<USRWallRunComponent>("WallRunComponent");
    GrappleComponent = CreateDefaultSubobject<USRGrappleComponent>("GrappleComponent");

    AbilityManager = CreateDefaultSubobject<USRAbilityManager>("AbilityManager");
}



// Called when the game starts or when spawned
void ASRPlayerCharacter::BeginPlay() { 
    Super::BeginPlay(); 

    if (GrappleComponent)
    {
        GrappleComponent->AttachCableToSocket(FirstPersonMesh, TEXT("grapple_hook_socket"));
    }
}

void ASRPlayerCharacter::Landed(const FHitResult &Hit)
{
    Super::Landed(Hit);
    WallRunComponent->StopWallRun();
}

// Called every frame
void ASRPlayerCharacter::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input
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
    CurrentZoneFlags = InFlags;
    GrappleComponent->SetAllowGrapple(InFlags.bCanGrapple);
    GrappleComponent->UpdateGrappleCheckTimer();
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
        if (WallRunComponent->IsWallRunning())
        {
            WallRunComponent->StopWallRun();
        }
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
        if (bIsDashing)
        {
            const float MoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
            FVector DeltaMove = RightDirection * MovementVector.X * MoveSpeed * GetWorld()->GetDeltaSeconds();
            AddActorWorldOffset(DeltaMove, true);
            // Use AddMovementInput to trigger character animations correctly.
            // AddActorWorldOffset only moves the actor in world space and bypasses the animation system.
            // Note: If time is slowed down, relying solely on AddMovementInput may result in slower movement.
            // Combining both ensures visual correctness and consistent movement behavior.
            AddMovementInput(RightDirection, MovementVector.X * 0.1);
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
        if (WallRunComponent->IsWallRunning())
        {
            WallRunComponent->StopWallRun();
            Debug::Print("Jump DuringWallRun");
            FVector PlaneContraintNormal = GetCharacterMovement()->GetPlaneConstraintNormal();
            // launch character based on its speeed
            const float DashStrength = GetVelocity().Length();
            FVector UpwardForce = FVector(0, 0, DashStrength);
            LaunchCharacter(PlaneContraintNormal * DashStrength + UpwardForce, false, true);
        }
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
        StartDashing();
    }
    else
    {
        StopDashing();
    }
}

void ASRPlayerCharacter::Input_Grapple(const FInputActionValue &InputActionValue)
{
    if (InputActionValue.Get<bool>())
    {
        FHitResult Results = GrappleComponent->CheckForGrapplePoints();
        GrappleComponent->Grapple(Results);
    }
    else
    {
        GrappleComponent->ResetGrappleState();
    }
}

#pragma endregion

#pragma region Dash
void ASRPlayerCharacter::StartDashing()
{
    if (WallRunComponent->IsWallRunning())
    {
        WallRunComponent->StopWallRun();
    }
    if (GetCharacterMovement()->IsFalling())
    {
        bIsDashing = true;
        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
    }
}

void ASRPlayerCharacter::StopDashing()
{
    bIsDashing = false;
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
    const FVector DashDir = Controller->GetControlRotation().Vector();
    const float DashStrength = 1000.0f;
    LaunchCharacter(DashDir * DashStrength, true, true);
}
#pragma endregion

#pragma region Grapple

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
    if (WallRunComponent->CheckForWall(Hit))
    {
        WallRunComponent->StartWallRun(Hit);
    }
    else
    {
        WallRunComponent->StopWallRun();
    }
}