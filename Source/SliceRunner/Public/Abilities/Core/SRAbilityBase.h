// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SRAbilityBase.generated.h"

struct FSRAbilityActivationContext;
class USRAbilityManager;
/**
 *
 */
UCLASS(Blueprintable)
class SLICERUNNER_API USRAbilityBase : public UObject
{
    GENERATED_BODY()
  protected:

    UFUNCTION()
    USRAbilityManager *GetOwningComponent() const;

    UPROPERTY(EditDefaultsOnly, Category = "Ability|Tags")
    FGameplayTagContainer GrantTags;

    UPROPERTY(EditDefaultsOnly, Category = "Ability|Tags")
    FGameplayTagContainer BlockTags;

  public:
    virtual void StartAbility(AActor *Instigator, const FSRAbilityActivationContext &Context);
    virtual void StopAbility(AActor *Instigator);

    UPROPERTY(EditAnywhere, Category = "Ability")
    FName AbilityName;
};
