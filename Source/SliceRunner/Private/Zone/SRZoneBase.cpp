// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "Zone/SRZoneBase.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter/SRPlayerCharacter.h"
#include "Debug/DebugHelper.h"
// Sets default values
ASRZoneBase::ASRZoneBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    ZoneTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    RootComponent = ZoneTrigger;
    ZoneTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ZoneTrigger->SetCollisionObjectType(ECC_WorldStatic);
    ZoneTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    ZoneTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    ZoneTrigger->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASRZoneBase::BeginPlay()
{
    Super::BeginPlay();
    ZoneTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASRZoneBase::OnOverlapBegin);
    // ZoneTrigger->OnComponentEndOverlap.AddDynamic(this, &ASRZoneBase::OnOverlapEnd);
}

// Called every frame
void ASRZoneBase::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

const FZoneAbilityFlags &ASRZoneBase::GetZoneAbilities() const { return AbilityFlags; }

void ASRZoneBase::OnOverlapBegin(
    UPrimitiveComponent *OverlappedComp,
    AActor *OtherActor,
    UPrimitiveComponent *OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult &SweepResult
)
{
    if (!OtherActor || OtherActor == this)
        return;

    ASRPlayerCharacter *Player = Cast<ASRPlayerCharacter>(OtherActor);
    if (Player)
    {
        const FZoneAbilityFlags &ZoneFlags = GetZoneAbilities();
        if (ZoneFlags.IsEmpty())
        {
            Player->ClearZoneFlags();
        }
        else
        {
            Player->SetZoneFlags(ZoneFlags);
        }

        UE_LOG(LogTemp, Warning, TEXT("ZoneAbilities: %d"), GetZoneAbilities().Flags);


    }
}
