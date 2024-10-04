#pragma once

#include "CoreMinimal.h"


DECLARE_LOG_CATEGORY_EXTERN(Custom, Log, All);


namespace Debugging {

	template<typename... args>
	inline constexpr void CustomLog(const FString& message, args&&... arguments) noexcept { UE_LOG(Custom, Log, TEXT("%s"), *message, arguments...); }

	template<typename... args>
	inline constexpr void CustomWarning(const FString& message, args&&... arguments) noexcept { UE_LOG(Custom, Warning, TEXT("%s"), *message, arguments...); }

	template<typename... args>
	inline constexpr void CustomError(const FString& message, args&&... arguments) noexcept { UE_LOG(Custom, Error, TEXT("%s"), *message, arguments...); }

	void DebugLine(const UWorld* world, const FVector& start, const FVector& end, const FColor& color, bool persistentLines = false, float lifetime = -1.0f) noexcept;
	void PrintString(const FString& message, int32 key = -1, float timeToDisplay = 5.0f, FColor color = FColor::Red, bool newerOnTop = true) noexcept;
}