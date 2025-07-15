// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "Abilities/Dash/SRDashComponent.h"

USRDashComponent::USRDashComponent() { PrimaryComponentTick.bCanEverTick = false; }

void USRDashComponent::BeginPlay() { Super::BeginPlay(); }

void USRDashComponent::TickComponent(
    float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
