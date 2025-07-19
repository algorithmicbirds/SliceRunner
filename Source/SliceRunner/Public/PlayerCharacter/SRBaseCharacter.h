// Copyright (c) 2025 algorithmicbird. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SRBaseCharacter.generated.h"

class USRAbilitySystemComponent;

UCLASS()
class SLICERUNNER_API ASRBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

  public:
    ASRBaseCharacter();

  protected:
    virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;
    virtual void PossessedBy(AController *NewController) override;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ablity")
    TObjectPtr<USRAbilitySystemComponent> SRAbilitySystemComponent;

  public:
    FORCEINLINE USRAbilitySystemComponent *GetSRAblitySystemComponent() const { return SRAbilitySystemComponent; }
};
