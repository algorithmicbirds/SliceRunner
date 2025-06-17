// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#include "Debug/DebugHelper.h"
#include "Engine/Engine.h"

namespace Debug
{

void Print(const FString &Msg, FColor Color)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, Msg);
    }
}

void Print(const float &InFloat, FColor Color)
{
    if (GEngine)
    {
        const FString DebugString = FString::Printf(TEXT("%.2f"), InFloat);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, DebugString);
    }
}

void Print(const FVector &InVector, FColor Color)
{
    if (GEngine)
    {
        const FString DebugString =
            FString::Printf(TEXT("Vector: X=%.2f Y=%.2f Z=%.2f"), InVector.X, InVector.Y, InVector.Z);

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, DebugString);
    }
}

void Print(const FString &Msg, const FVector &InVector, FColor Color)
{
    if (GEngine)
    {
        const FString DebugString =
            FString::Printf(TEXT("%s Vector: X=%.2f Y=%.2f Z=%.2f"), *Msg, InVector.X, InVector.Y, InVector.Z);

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, Color, DebugString);
    }
}

} // namespace Debug
