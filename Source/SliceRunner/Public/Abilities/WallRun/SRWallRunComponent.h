// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRWallRunComponent.generated.h"

class USRRaycastSensor;
class ASRBaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLICERUNNER_API USRWallRunComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USRWallRunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
  UPROPERTY()
  USRRaycastSensor *RaycastSensorInnerWall;

  UPROPERTY()
  USRRaycastSensor *RaycastSensorOuterWall;

public:
  // Called every frame
  virtual void
  TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
  void StartWallRun(const FHitResult &Hit);
  void StopWallRun();
  bool CheckForWall(const FHitResult &Hit);
  bool IsWallRunning() const { return bIsWallRunning; }

private:
  void EvaluateWallRunStateWithWallChecks();
  void CheckWallRunEndDueToNoMovement();
  bool bIsWallRunning = false;
  FTimerHandle WallRunCheckTimerHandle;
  FTimerHandle WallRunMovementCheckHandle;
  UPROPERTY()
  ASRBaseCharacter *Character;
};
