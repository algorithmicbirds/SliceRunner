// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Gate/GateRuleInterface.h"
#include "SRAbilityActivationContext.generated.h"

USTRUCT(BlueprintType)
struct FSRAbilityActivationContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FHitResult HitResults;

    UPROPERTY(BlueprintReadOnly)
    FGateAbilityFlags GateFlags;
};
