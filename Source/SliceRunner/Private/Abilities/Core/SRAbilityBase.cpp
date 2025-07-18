// Copyright (c) 2025 algorithmicbird. All rights reserved.

#include "Abilities/Core/SRAbilityBase.h"
#include "Abilities/Core/SRAbilityActivationContext.h"
#include "Abilities/Core/SRAbilityManager.h"
#include "Debug/DebugHelper.h"


void USRAbilityBase::StartAbility(AActor *Instigator, const FSRAbilityActivationContext &Context) {
    // Debug::Print(FString("Start Ability: ") + FString(GetNameSafe(this)));
    USRAbilityManager *AbilityManager = GetOwningComponent();
    AbilityManager->ActiveAbilities.AppendTags(GrantTags);
}

void USRAbilityBase::StopAbility(AActor *Instigator) {
    // Debug::Print(FString("Stop Ability: ") + FString(GetNameSafe(this)));
    USRAbilityManager *AbilityManager = GetOwningComponent();
    AbilityManager->ActiveAbilities.RemoveTags(GrantTags);
}

USRAbilityManager *USRAbilityBase::GetOwningComponent() const { return CastChecked<USRAbilityManager>(GetOuter()); }