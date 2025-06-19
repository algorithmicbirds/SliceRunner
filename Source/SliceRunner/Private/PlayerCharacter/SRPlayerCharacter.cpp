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

ASRPlayerCharacter::ASRPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(32.f, 96.f);

    FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
    FirstPersonMesh->SetupAttachment(GetMesh());
    FirstPersonMesh->SetOnlyOwnerSee(true);
    FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
    FirstPersonMesh->SetCollisionProfileName(TEXT("NoCollision"));

    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
    FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, TEXT("head"));
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;
    GetCharacterMovement()->AirControl = 1.f;
    GetCharacterMovement()->JumpZVelocity = 600.f;
}

// Called when the game starts or when spawned
void ASRPlayerCharacter::BeginPlay() { Super::BeginPlay(); }

void ASRPlayerCharacter::Landed(const FHitResult &Hit)
{
    Super::Landed(Hit);
    StopWallRun();
}

// Called every frame
void ASRPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //Debug::Print(TEXT("Velocity: "), static_cast<float>(GetVelocity().Length()), 2);
    if (GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero() && bIsWallRunning)
    {   
        //Debug::Print(TEXT("Actor Forward Vector"), GetActorForwardVector(), 1);
        StopWallRun();
    }
}

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
}

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
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
        if (bIsDashing)
        {
            const float MoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
            FVector DeltaMove = RightDirection * MovementVector.X * MoveSpeed * GetWorld()->GetDeltaSeconds();
            AddActorWorldOffset(DeltaMove, true);
            // for making animations work; slowing down time affects movement and slows down input
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
        if (bIsWallRunning)
        {
            StopWallRun();
            FVector PlaneContraintNormal = GetCharacterMovement()->GetPlaneConstraintNormal();
            // launch character based on its speeed
            const float DashStrength = GetVelocity().Length();
            float Speed = 1.0f;
            FVector LaunchZDirection = FVector(0, 0, DashStrength);
            LaunchCharacter(PlaneContraintNormal * DashStrength * Speed + LaunchZDirection, false, true);
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

void ASRPlayerCharacter::StartDashing()
{
    if (bIsWallRunning)
    {
        StopWallRun();
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
    const FRotator ControlRot(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    const FVector ForwardDir = ControlRot.Vector();
    const float DashStrength = 1500.f;
    LaunchCharacter(ForwardDir * DashStrength, true, true);
}

bool ASRPlayerCharacter::CheckForWall(const FHitResult &Hit)
{
    float CharacterAndWallAlignment = FMath::Abs(FVector::DotProduct(Hit.ImpactNormal, GetActorRightVector()));
    if (CharacterAndWallAlignment > 0.7)
    {
        return true;
    }
    if (CharacterAndWallAlignment < 0.7)
    {
        return false;
    }
    return false;
}

void ASRPlayerCharacter::StartWallRun(const FHitResult &Hit)
{
    bIsWallRunning = true;
    if (GetCharacterMovement()->IsFalling())
    {
        GetCharacterMovement()->SetMovementMode(MOVE_Flying);
        GetCharacterMovement()->SetPlaneConstraintEnabled(true);
        GetCharacterMovement()->SetPlaneConstraintNormal(Hit.ImpactNormal);
    }
}

void ASRPlayerCharacter::StopWallRun()
{
    bIsWallRunning = false;
    GetCharacterMovement()->SetPlaneConstraintEnabled(false);
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

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
    if (CheckForWall(Hit))
    {
        StartWallRun(Hit);
    }
    else
    {
        StopWallRun();
    }
}
