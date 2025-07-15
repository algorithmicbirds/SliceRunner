// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "Abilities/Core/SRAbilityBase.h"
#include "Abilities/Core/SRAbilityActivationContext.h"
#include "Debug/DebugHelper.h"

void USRAbilityBase::StartAbility_Implementation(AActor *Instigator, const FSRAbilityActivationContext &Context)
{
    //Debug::Print(FString("Start Ability: ") + FString(GetNameSafe(this)));
}

void USRAbilityBase::StopAbility_Implementation(AActor *Instigator)
{
    //Debug::Print(FString("Stop Ability: ") + FString(GetNameSafe(this)));
}

