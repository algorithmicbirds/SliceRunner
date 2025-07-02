// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "UI/GrapplePoint/SRGrapplePoint.h"
#include "UI/GrapplePoint/SRGrapplePointWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
ASRGrapplePoint::ASRGrapplePoint()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
}

// Called when the game starts or when spawned
void ASRGrapplePoint::BeginPlay() { 
    Super::BeginPlay();
   
}

void ASRGrapplePoint::SetGrappleIconVisible(bool Visible)
{
    if (UUserWidget *Widget = WidgetComponent->GetUserWidgetObject())
    {
        USRGrapplePointWidget *GrappleWidget = Cast<USRGrapplePointWidget>(Widget);
        if (GrappleWidget)
        {
            GrappleWidget->SetGrappleIconVisible(Visible);
        }
    }
}

// Called every frame
void ASRGrapplePoint::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
