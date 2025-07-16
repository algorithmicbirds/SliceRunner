// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GateRuleInterface.generated.h"

USTRUCT(BlueprintType)
struct FGateAbilityFlags
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gate Ability")
    bool bCanGrapple = false;

};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGateRuleInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 * Gate interface that exposes ability flags
 */
class SLICERUNNER_API IGateRuleInterface
{
    GENERATED_BODY()

  public:
    virtual const FGateAbilityFlags &GetGateAbilities() const = 0;
};
