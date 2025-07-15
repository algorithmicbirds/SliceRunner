// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "Abilities/Core/SRAbilityManager.h"
#include "Abilities/Core/SRAbilityBase.h"
#include "Abilities/Core/SRAbilityActivationContext.h"
#include "Debug/DebugHelper.h"

USRAbilityManager::USRAbilityManager() { PrimaryComponentTick.bCanEverTick = false; }

void USRAbilityManager::BeginPlay()
{
    Super::BeginPlay();
    for (TSubclassOf<USRAbilityBase> AbilityClass : DefaultAbilities)
    {
        AddAbility(GetOwner(), AbilityClass);
    }
}

void USRAbilityManager::TickComponent(
    float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction
)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USRAbilityManager::AddAbility(AActor *Instigator, TSubclassOf<USRAbilityBase> AbilityClass)
{
    if (!ensure(AbilityClass))
    {
        return;
    }
    USRAbilityBase *NewAbility = NewObject<USRAbilityBase>(this, AbilityClass);
    if (NewAbility)
    {
        Abilities.Add(NewAbility);
    }
}

bool USRAbilityManager::StartAbilityByName(AActor *Instigator, FName AbilityName, const FSRAbilityActivationContext &Context)
{
    for (USRAbilityBase *Ability : Abilities)
    {
        if (Ability && Ability->AbilityName == AbilityName)
        {
            Ability->StartAbility(Instigator, Context);
            return true;
        }
    }

    return false;
}

bool USRAbilityManager::StopAbilityByName(AActor *Instigator, FName AbilityName)
{
    for (USRAbilityBase *Ability : Abilities)
    {
        if (Ability && Ability->AbilityName == AbilityName)
        {
            Ability->StopAbility(Instigator);
            return true;
        }
    }
    return false;
}
