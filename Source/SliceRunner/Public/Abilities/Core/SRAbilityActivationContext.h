// Copyright (c) 2025 algorithmicbird. All rights reserved.

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
