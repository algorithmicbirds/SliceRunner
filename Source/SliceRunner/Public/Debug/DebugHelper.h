#pragma once

#include "CoreMinimal.h"

namespace Debug
{
#if !UE_SHIPPING_BUILD
void Print(const FString &Msg, int32 key = -1, FColor Color = FColor::MakeRandomColor());
void Print(const float &InFloat, int32 key = -1, FColor Color = FColor::MakeRandomColor());
void Print(const FString &Msg, const float &InFloat, int32 key = -1, FColor Color = FColor::MakeRandomColor());
void Print(const FVector &InVector, int32 key = -1, FColor Color = FColor::MakeRandomColor());
void Print(const FString &Msg, const FVector &InVector, int32 key = -1, FColor Color = FColor::MakeRandomColor());
#endif
} // namespace Debug
