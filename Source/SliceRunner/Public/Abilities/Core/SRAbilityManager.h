// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SRAbilityManager.generated.h"

class USRAbilityBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLICERUNNER_API USRAbilityManager : public UActorComponent
{
    GENERATED_BODY()

  public:
    USRAbilityManager();

  protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    TArray<TObjectPtr<USRAbilityBase>> Abilities;

  public:
    virtual void
    TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Ability")
    void AddAbility(AActor *Instigator, TSubclassOf<USRAbilityBase> AbilityClass);

    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool StartAbilityByName(AActor *Instigator, FName AbilityName);

    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool StopAbilityByName(AActor *Instigator, FName AbilityName);
};
