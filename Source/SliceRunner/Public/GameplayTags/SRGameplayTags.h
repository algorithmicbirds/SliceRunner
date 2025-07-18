// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace SRGameplayTags
{
    namespace InputTags
    {
        SLICERUNNER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Move);
        SLICERUNNER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Look);
        SLICERUNNER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Jump);
        SLICERUNNER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dash);
        SLICERUNNER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Grapple);
    } // namespace InputTags
    namespace AbilityTags
    {
        namespace MovementTags
        {
            SLICERUNNER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WallRun);
            SLICERUNNER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dash);
            SLICERUNNER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Grapple);
        } // namespace MovementTags
    } // namespace AbilityTags
} // namespace SRGameplayTags
