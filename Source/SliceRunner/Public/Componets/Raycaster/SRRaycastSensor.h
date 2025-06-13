// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SRRaycastSensor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLICERUNNER_API USRRaycastSensor : public USceneComponent
{
	GENERATED_BODY()

public:
	float CastLength = 1000.0f;
	FVector Origin = FVector::ZeroVector;
	FVector CastDirection = FVector::ForwardVector;
	void Cast();
	void DebugCast();

	void SetCastDirection(FVector direction) { CastDirection = direction; }
	void SetCastOrigin(FVector Pos) { Origin = Pos; }
	bool HasDetectedHit() const { return hitInfo.bBlockingHit; }
	float GetDistance() const { return hitInfo.Distance; }
	FVector GetNormal() const { return hitInfo.ImpactNormal; }
	FVector GetPosition() const { return hitInfo.ImpactPoint; }
	FTransform GetTransform() const
	{
		if (hitInfo.GetActor())
		{
			return hitInfo.GetActor()->GetActorTransform();
		}
		return FTransform::Identity;
	}



private:
	FHitResult hitInfo;
		
};
