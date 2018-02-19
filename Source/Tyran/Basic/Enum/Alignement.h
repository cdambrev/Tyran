#pragma once
#include "UObjectGlobals.h"


UENUM(BlueprintType)
enum class EAlignement : uint8 {
	A_TYRAN UMETA(DisplayName = "Tyran"),
	A_REVOLUTIONNAIRE UMETA(DisplayName = "Revolutionnaire")


};

static FString EAlignementToString(EAlignement e) {
	FString s[] = { "Tyran","Revolutionnaire" };
	uint8 index = (uint8)e;
	return s[index];
}