
#include "GP4Testing/Utility/Timer.h"
#include "GP4Testing/Utility/Debugging.h"

Timer::Timer(float value, Callback onCompleted) noexcept {
	Setup(value, onCompleted);
}
Timer::Timer(const float* ref, Callback onCompleted) noexcept {
	Setup(ref, onCompleted);
}


void Timer::Setup(float value, Callback onCompleted) noexcept {
	SetLength(value);
	SetOnCompletedCallback(onCompleted);
}
void Timer::Setup(const float* ref, Callback onCompleted) noexcept {
	SetLengthRef(ref);
	SetOnCompletedCallback(onCompleted);
}
void Timer::SetLength(float value) noexcept { 
	length = value; 
	Reevaluate();
}
void Timer::SetLengthRef(const float* ref) noexcept {
	lengthRef = ref; 
	Reevaluate();
}
void Timer::Reevaluate() noexcept {
	if (lengthRef && *lengthRef > 0.0f)
		valid = true;
	else if (length > 0.0f)
		valid = true;
	else
		valid = false;
}


void Timer::ClearAllData() noexcept {
	valid = false;
	currentTime = 0.0f;
	length = 0.0f;
	lengthRef = nullptr;
	onCompletedCallback = nullptr;
}
void Timer::ResetTime() noexcept {
	currentTime = 0.0f;
}


void Timer::Update(float deltaTime) noexcept {
	if (!IsValid() || isLocked)
		return;

	UpdateTime(deltaTime);
}
void Timer::UpdateTime(float deltaTime) noexcept {
	float comparisonLength = 0.0f;
	if (lengthRef)
		comparisonLength = *lengthRef;
	else
		comparisonLength = length;

	currentTime += deltaTime;
	if (currentTime >= comparisonLength) {
		currentTime -= comparisonLength;
		if (onCompletedCallback)
			onCompletedCallback();
		if (triggerOnce)
			isLocked = true;
	}
}