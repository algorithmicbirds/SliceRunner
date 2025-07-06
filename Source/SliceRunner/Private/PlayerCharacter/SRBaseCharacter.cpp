// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.


#include "PlayerCharacter/SRBaseCharacter.h"

// Sets default values
ASRBaseCharacter::ASRBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASRBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASRBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASRBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

