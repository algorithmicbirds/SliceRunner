// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
    UFUNCTION(BlueprintNativeEvent, Category = "Ability")
    void StartAbility(AActor *Instigator, const FSRAbilityActivationContext &Context);

    UFUNCTION(BlueprintNativeEvent, Category = "Ability")
    void StopAbility(AActor *Instigator);

    UPROPERTY(EditAnywhere, Category = "Ability")
    FName AbilityName;
};
