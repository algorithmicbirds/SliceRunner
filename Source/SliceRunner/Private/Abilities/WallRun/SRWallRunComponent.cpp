// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.


#include "Abilities/WallRun/SRWallRunComponent.h"
#include "Components/Raycaster/SRRaycastSensor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
USRWallRunComponent::USRWallRunComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
    // features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    RaycastSensorInnerWall = CreateDefaultSubobject<USRRaycastSensor>(TEXT("RaycastSensorInnerWall"));
    RaycastSensorOuterWall = CreateDefaultSubobject<USRRaycastSensor>(TEXT("RaycastSensorOuterWall"));
}


// Called when the game starts
void USRWallRunComponent::BeginPlay()
{
	Super::BeginPlay();
    Character = Cast<ACharacter>(GetOwner());
	
}


// Called every frame
void USRWallRunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USRWallRunComponent::CheckForWall(const FHitResult &Hit)
{
    if (!Character)
        return false;
    float CharacterAndWallAlignment = FMath::Abs(FVector::DotProduct(Hit.ImpactNormal, Character->GetActorRightVector()));
    return CharacterAndWallAlignment > 0.7f;
}

void USRWallRunComponent::StartWallRun(const FHitResult &Hit)
{
    if (!Character)
        return;
    bIsWallRunning = true;
    if (Character->GetCharacterMovement()->IsFalling())
    {
        Character->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
        Character->GetCharacterMovement()->SetPlaneConstraintEnabled(true);
        Character->GetCharacterMovement()->SetPlaneConstraintNormal(Hit.ImpactNormal);
    }

    GetWorld()->GetTimerManager().SetTimer(
        WallRunCheckTimerHandle, this, &USRWallRunComponent::EvaluateWallRunStateWithWallChecks, 0.1f, true
    );
    GetWorld()->GetTimerManager().SetTimer(
        WallRunMovementCheckHandle, this, &USRWallRunComponent::CheckWallRunEndDueToNoMovement, 0.1f, true
    );
}

void USRWallRunComponent::StopWallRun()
{
    if (!Character)
        return;
    bIsWallRunning = false;
    Character->GetCharacterMovement()->SetPlaneConstraintEnabled(false);
    Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    GetWorld()->GetTimerManager().ClearTimer(WallRunCheckTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(WallRunMovementCheckHandle);
}

void USRWallRunComponent::EvaluateWallRunStateWithWallChecks()
{
    if (!bIsWallRunning || !Character)
    {
        return;
    }

    FVector CastOrigin = Character->GetActorLocation();

    auto PerformRayCast = [&CastOrigin](USRRaycastSensor *RaycasterSensor, FVector CastDirection, float Length)
    {
        RaycasterSensor->SetCastOrigin(CastOrigin);
        RaycasterSensor->SetCastDirection(CastDirection);
        RaycasterSensor->CastLength = Length;
        RaycasterSensor->Cast();
    };

    FVector CastDirectionInwards = Character->GetActorForwardVector();
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
            FVector PlaneConstraintNormal = Character->GetCharacterMovement()->GetPlaneConstraintNormal();
            float DashStrength = 400.0f;
            FVector LateralDashForce = FVector(0.0f, DashStrength, 0.0f);
            Character->LaunchCharacter(PlaneConstraintNormal * DashStrength + LateralDashForce, true, false);
        }
    }

    // Check if player has moved too far away from all side walls (i.e., is no longer adjacent to any wall)
    PerformRayCast(RaycastSensorOuterWall, Character->GetActorRightVector(), 60.0f);
    bool bRightClear = !RaycastSensorOuterWall->HasDetectedHit();

    PerformRayCast(RaycastSensorOuterWall, -(Character->GetActorRightVector()), 60.0f);
    bool bLeftClear = !RaycastSensorOuterWall->HasDetectedHit();

    // If both left and right sides are clear, player has lost wall contact—end wall run
    if (bRightClear && bLeftClear && bIsWallRunning)
    {
        StopWallRun();
    }
}

void USRWallRunComponent::CheckWallRunEndDueToNoMovement()
{
    if (!bIsWallRunning || !Character)
        return;

    if (Character->GetCharacterMovement()->GetCurrentAcceleration().IsNearlyZero())
    {
        StopWallRun();
    }
}
