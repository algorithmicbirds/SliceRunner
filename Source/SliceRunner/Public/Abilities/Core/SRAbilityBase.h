// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SRAbilityBase.generated.h"

struct FSRAbilityActivationContext;
/**
 *
 */
UCLASS(Blueprintable)
class SLICERUNNER_API USRAbilityBase : public UObject
{
    GENERATED_BODY()
  public:
    virtual void StartAbility(AActor *Instigator, const FSRAbilityActivationContext &Context);
    virtual void StopAbility(AActor *Instigator);
};
