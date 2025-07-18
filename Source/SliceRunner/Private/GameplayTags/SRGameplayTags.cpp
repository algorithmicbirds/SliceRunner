// Copyright (c) 2025 algorithmicbird. All rights reserved.

#include "GameplayTags/SRGameplayTags.h"

namespace SRGameplayTags
{
    namespace InputTags
    {

        UE_DEFINE_GAMEPLAY_TAG(Move, "InputTag.Move");
        UE_DEFINE_GAMEPLAY_TAG(Look, "InputTag.Look");
        UE_DEFINE_GAMEPLAY_TAG(Jump, "InputTag.Jump");
        UE_DEFINE_GAMEPLAY_TAG(Dash, "InputTag.Dash");
        UE_DEFINE_GAMEPLAY_TAG(Grapple, "InputTag.Grapple");
    } // namespace InputTags
    namespace AbilityTags
    {
        namespace Movement
        {
            UE_DEFINE_GAMEPLAY_TAG(Grapple, "AbilityTag.Movement.Grapple");
            UE_DEFINE_GAMEPLAY_TAG(WallRun, "AbilityTag.Movement.WallRun");
            UE_DEFINE_GAMEPLAY_TAG(Dash, "AbilityTag.Movement.Dash");
        } // namespace Movement
    } // namespace AbilityTags
} // namespace SRGameplayTags
