// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SRAbilityBase.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SLICERUNNER_API USRAbilityBase : public UObject
{
    GENERATED_BODY()
  public:
    UFUNCTION(BlueprintNativeEvent, Category = "Ability")
    void StartAbility(AActor *Instigator);
    
    UFUNCTION(BlueprintNativeEvent, Category = "Ability")
    void StopAbility(AActor *Instigator);
    
    UPROPERTY(EditAnywhere, Category = "Ability")
    FName AbilityName;
};
