// Copyright (c) 2025 algorithmicbird. All rights reserved.

#include "PlayerCharacter/SRBaseCharacter.h"
#include "Abilities/Core/SRAbilitySystemComponent.h"

// Sets default values
ASRBaseCharacter::ASRBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    SRAbilitySystemComponent = CreateDefaultSubobject<USRAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}


void ASRBaseCharacter::PossessedBy(AController *NewController)
{
    if (SRAbilitySystemComponent)
    {
        SRAbilitySystemComponent->InitAbilityActorInfo(this, this);
    }
}

UAbilitySystemComponent *ASRBaseCharacter::GetAbilitySystemComponent() const { return GetSRAblitySystemComponent(); }