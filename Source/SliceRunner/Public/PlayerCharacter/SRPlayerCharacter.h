// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "SRPlayerCharacter.generated.h"

class UCameraComponent;
class USRRaycastSensor;
class USRDataAsset_InputConfig;

UCLASS()
class SLICERUNNER_API ASRPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASRPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
#pragma region Components

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USRRaycastSensor* Sensor;

#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	USRDataAsset_InputConfig* InputDataAssetConfig;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);
	void Input_Dash(const FInputActionValue& InputActionValue);

#pragma endregion

#pragma region Actions
	void StartDashing();
	void StopDashing();
	void CheckForWall();
	void StartWallRun();
	void StopWallRun();
#pragma endregion

public:
	bool bIsDashing = false;

private:
	float WallCheckInterval = 0.1f;
	float WallCheckTime = 0.0f;
	bool bIsWallRunning = false;
};
