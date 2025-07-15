// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "SRAbilityActivationContext.generated.h"

USTRUCT(BlueprintType)
struct FSRAbilityActivationContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FHitResult HitResults;
};