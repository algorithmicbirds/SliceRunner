// Copyright (c) 2025 algorithmicbird. All rights reserved.

#include "World/GrapplePoint/SRGrapplePointWidget.h"
#include "Components/Image.h"

void USRGrapplePointWidget::NativeConstruct()
{
    Super::NativeConstruct();
    SetGrappleIconVisible(false);
}

void USRGrapplePointWidget::SetGrappleIconVisible(bool Visible)
{
    if (GrappleIconAllowed && GrappleIconNotAllowed)
    {
        GrappleIconAllowed->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        GrappleIconNotAllowed->SetVisibility(Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
}
