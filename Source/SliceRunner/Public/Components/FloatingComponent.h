// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingComponent.generated.h"

UCLASS()
class SLICERUNNER_API AFloatingComponent : public AActor
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    AFloatingComponent();

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

  private:
    FVector StartLocation;
    float ElapsedTime;
};
