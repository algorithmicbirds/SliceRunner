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
#include "Components/Raycaster/SRRaycastSensor.h"


ASRPlayerCharacter::ASRPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(32.f, 96.f);

    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
    FirstPersonMesh->SetOnlyOwnerSee(true);
    FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
    FirstPersonCameraComponent->SetupAttachment(GetMesh(), TEXT("head"));
    FirstPersonMesh->SetCollisionProfileName(TEXT("NoCollision"));

    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
    GetCharacterMovement()->AirControl = 1.0f;
    GetCharacterMovement()->JumpZVelocity = 600.0f;

    RaycastSensorInnerWall = CreateDefaultSubobject<USRRaycastSensor>(TEXT("RaycastSensor Inner Wall"));
    RaycastSensorOuterWall = CreateDefaultSubobject<USRRaycastSensor>(TEXT("RaycastSensor Outer Wall"));
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

    // use raycast every 100 ms
    TimeEsplased += DeltaTime;
    if (TimeEsplased >= TimeIntervel)
    {
        EvaluateWallRunStateWithWallChecks();
        TimeEsplased = 0.0f;
    }

    if (GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero() && bIsWallRunning)
    {
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
        if (bIsWallRunning)
        {
            StopWallRun();
        }
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

void ASRPlayerCharacter::EvaluateWallRunStateWithWallChecks()
{
    if (!bIsWallRunning)
    {
        return;
    }

    FVector CastOrigin = GetActorLocation();

    auto PerformRayCast = [&CastOrigin](USRRaycastSensor *RaycasterSensor, FVector CastDirection, float Length)
    {
        RaycasterSensor->SetCastOrigin(CastOrigin);
        RaycasterSensor->SetCastDirection(CastDirection);
        RaycasterSensor->CastLength = Length;
        RaycasterSensor->Cast(); 
    };

    FVector CastDirectionInwards = GetActorForwardVector();
    PerformRayCast(RaycastSensorInnerWall, CastDirectionInwards, 150.0f);

    if (RaycastSensorInnerWall->HasDetectedHit())
    {
        // Determine how closely the player's movement direction aligns with the wall surface
        FVector WallNormal = RaycastSensorInnerWall->GetNormal();
        float PlayerAndObstacleAlignment = FMath::Abs(FVector::DotProduct(WallNormal, CastDirectionInwards));

        // If alignment is high, player is facing the wall too directly—cancel the wall run
        if (PlayerAndObstacleAlignment > 0.8 && bIsWallRunning)
        {
            StopWallRun();

            // Apply a short directional push away from the wall to keep momentum fluid
            FVector PlaneConstraintNormal = GetCharacterMovement()->GetPlaneConstraintNormal();
            float DashStrength = 400.0f;
            FVector LateralDashForce = FVector(0.0f, DashStrength, 0.0f);
            LaunchCharacter(PlaneConstraintNormal * DashStrength + LateralDashForce, true, false);
        }
    }

    // Check if player has moved too far away from all side walls (i.e., is no longer adjacent to any wall)
    PerformRayCast(RaycastSensorOuterWall, GetActorRightVector(), 60.0f);
    bool bRightClear = !RaycastSensorOuterWall->HasDetectedHit();

    PerformRayCast(RaycastSensorOuterWall, -GetActorRightVector(), 60.0f);
    bool bLeftClear = !RaycastSensorOuterWall->HasDetectedHit();

    // If both left and right sides are clear, player has lost wall contact—end wall run
    if (bRightClear && bLeftClear && bIsWallRunning)
    {
        StopWallRun();
    }
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
