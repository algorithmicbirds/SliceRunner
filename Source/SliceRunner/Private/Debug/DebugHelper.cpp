// Copyright (c) 2025 algorithmicbird. All rights reserved.

#include "Debug/DebugHelper.h"
#include "Engine/Engine.h"

namespace Debug
{

void Print(const FString &Msg,int32 key, FColor Color)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(key, 5.0f, Color, Msg);
    }
}

void Print(const float &InFloat,int32 key, FColor Color)
{
    if (GEngine)
    {
        const FString DebugString = FString::Printf(TEXT("%.2f"), InFloat);
        GEngine->AddOnScreenDebugMessage(key, 5.0f, Color, DebugString);
    }
}

void Print(const FVector &InVector,int32 key, FColor Color)
{
    if (GEngine)
    {
        const FString DebugString =
            FString::Printf(TEXT("Vector: X=%.2f Y=%.2f Z=%.2f"), InVector.X, InVector.Y, InVector.Z);

        GEngine->AddOnScreenDebugMessage(key, 5.0f, Color, DebugString);
    }
}


void Print(const FString &Msg, const float &InFloat, int32 key, FColor Color) {
    if (GEngine)
    {
        const FString DebugString = FString::Printf(TEXT("%s : %.2f"), *Msg, InFloat);
        GEngine->AddOnScreenDebugMessage(key, 5.0f, Color, DebugString);
    }
}

void Print(const FString &Msg, const FVector &InVector, int32 key, FColor Color)
{
    if (GEngine)
    {
        const FString DebugString =
            FString::Printf(TEXT("%s Vector: X=%.2f Y=%.2f Z=%.2f"), *Msg, InVector.X, InVector.Y, InVector.Z);

        GEngine->AddOnScreenDebugMessage(key, 5.0f, Color, DebugString);
    }
}

void DrawSweepDebug(
    UWorld *World,
    const FVector &Start,
    const FVector &End,
    float Radius,
    const TArray<FHitResult> &Hits,
    float Duration
)
{
    if (!World)
        return;

    float LineThickness = 5.0f;
    DrawDebugSphere(World, End, Radius, 12, FColor::Red, false, Duration);
    DrawDebugLine(World, Start, End, FColor::Red, false, Duration, 0, LineThickness);

    for (const FHitResult &Hit : Hits)
    {
        DrawDebugLine(World, Start, Hit.ImpactPoint, FColor::Yellow, false, Duration, 0, LineThickness);
        DrawDebugSphere(World, Hit.ImpactPoint, Radius * 1.5f, 12, FColor::Yellow, false, Duration);
    }
}

void DrawLineDebug(UWorld *World, const FVector &Start, const FVector &End, const FHitResult &Hit, float Duration)
{
    if (!World)
        return;

    DrawDebugLine(World, Start, End, FColor::Red, false, Duration, 0, 2.0f);

    if (Hit.bBlockingHit)
    {
        DrawDebugLine(World, Start, Hit.ImpactPoint, FColor::Yellow, false, Duration, 0, 2.0f);
        DrawDebugSphere(World, Hit.ImpactPoint, 10.0f, 12, FColor::Yellow, false, Duration);
    }
}


} // namespace Debug
