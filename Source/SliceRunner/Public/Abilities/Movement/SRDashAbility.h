// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Core/SRAbilityBase.h"
#include "SRDashAbility.generated.h"

class ASRBaseCharacter;

UCLASS()
class SLICERUNNER_API USRDashAbility : public USRAbilityBase
{
    GENERATED_BODY()

  public:
    virtual void StartAbility(AActor *Instigator, const FSRAbilityActivationContext &Context) override;
    virtual void StopAbility(AActor *Instigator) override;

  protected:
    TObjectPtr<ASRBaseCharacter> Character;
};
