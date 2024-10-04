
#include "Debugging.h"


DEFINE_LOG_CATEGORY(Custom);


namespace Debugging {

	void DebugLine(const UWorld* world, const FVector& start, const FVector& end, const FColor& color, bool persistentLines, float lifetime) noexcept {
		DrawDebugLine(world, start, end, color, persistentLines, lifetime);
	}
	void PrintString(const FString& message, int32 key, float timeToDisplay, FColor color, bool newerOnTop) noexcept {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(key, timeToDisplay, color, message, newerOnTop);
	}
}