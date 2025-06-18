// Copyright (c) 2025 algorithmicbird  -- See MIT License for details.

#pragma once

#include "CoreMinimal.h"

namespace Debug
{
#if !UE_SHIPPING_BUILD
void Print(const FString &Msg, FColor Color = FColor::MakeRandomColor());
void Print(const float &InFloat, FColor Color = FColor::MakeRandomColor());
void Print(const FString &Msg, const float &InFloat, FColor Color = FColor::MakeRandomColor());
void Print(const FVector &InVector, FColor Color = FColor::MakeRandomColor());
void Print(const FString &Msg, const FVector &InVector, FColor Color = FColor::MakeRandomColor());
#else
inline void Print(const FString &, FColor) {}
inline void Print(const FVector &, FColor) {}
inline void Print(float &, FColor) {}
inline void Print(const FString &, float &, FColor) {}
inline void Print(const FString &, const FVector &, FColor) {}
#endif
} // namespace Debug
