// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZoneRuleInterface.generated.h"

UENUM(BlueprintType, meta=(Bitflags))
enum class EZoneAbility : uint8
{
    None = 0 UMETA(Hidden),
    CanGrapple = 1 << 0,
    CanWallRun = 1 << 1, 
};

ENUM_CLASS_FLAGS(EZoneAbility)

USTRUCT(BlueprintType)
struct FZoneAbilityFlags
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EZoneAbility"))
    int32 Flags = static_cast<int32>(EZoneAbility::None);

    bool IsEmpty() const { return Flags == 0; }

    bool Has(EZoneAbility Ability) const
    {
        return (Flags & static_cast<int32>(Ability)) == static_cast<int32>(Ability);
    }

    void Add(EZoneAbility Ability) { Flags |= static_cast<int32>(Ability); }

    void Remove(EZoneAbility Ability) { Flags &= ~static_cast<int32>(Ability); }

    EZoneAbility AsEnum() const { return static_cast<EZoneAbility>(Flags); }
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZoneRuleInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class SLICERUNNER_API IZoneRuleInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual const FZoneAbilityFlags &GetZoneAbilities() const = 0;
};
