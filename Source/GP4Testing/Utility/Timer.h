#pragma once

#include <functional>



//Instructions
// 1.Include
// Make sure to include "GP4Testing/Utility/Timer.h" and <functional> to use the timer.
// 
//2. Setup
//You can setup a Timer using either both custom constructors, the function Setup() or by setting the callback and timer manually using SetLength and SetOnCompletedCallback.
//Use Setup(float value, Callback onCompleted); if "value" is just a copy or a literal.
//Use Setup(const float* ref, Callback onCompleted); if "value" is a member variable that is to be edited in BP.
//Callback is set by doing std::bind(&Class::Function, this)).
// 
// 3. Usage
// To start the timer, call Update(deltaTime). You have full control over when does the timer get updated and by how much.
// The Timer is retriggerable by default.




class Timer final {
public:
	using Callback = std::function<void(void)>;

public:
	explicit Timer() noexcept = default;
	explicit Timer(float value, Callback onCompleted) noexcept;
	explicit Timer(const float* ref, Callback onCompleted) noexcept;

	operator bool() const noexcept { return valid; }
	explicit operator float() const noexcept { return currentTime; }

public:
	/// <summary>
	/// Sets up the timer using a float copy if is not valid, otherwise overwrites and attempts to validate again.
	/// </summary>
	void Setup(float value, Callback onCompleted) noexcept;

	/// <summary>
	/// Sets up the timer using a float reference if is not valid, otherwise overwrites and attempts to validate again.
	/// </summary>
	void Setup(const float* ref, Callback onCompleted) noexcept;

	/// <summary>
	/// Sets the current length copy value.
	/// Can also reevaluate whether the timer is valid or not.
	/// </summary>
	void SetLength(float value) noexcept;

	/// <summary>
	/// Sets the current length reference value.
	/// Can also reevaluate whether the timer is valid or not.
	/// </summary>
	void SetLengthRef(const float* ref) noexcept;

	/// <summary>
	/// Sets a callback to execute when the timer is completed.
	/// </summary>
	inline void SetOnCompletedCallback(Callback onCompleted) noexcept { onCompletedCallback = onCompleted; }

	/// <summary>
	/// Sets the value for whether this timer should require manual reactivation after finishing once.
	/// Sets isLocked to true after finishing.
	/// </summary>
	/// <param name="state"></param>
	inline void SetTriggerOnce(bool state) noexcept { triggerOnce = state; }

	/// <summary>
	/// Unlocks the timer after it has been locked by triggerOnce.
	/// </summary>
	inline void Unlock() noexcept { isLocked = false; }

	/// <summary>
	/// Clears all data in the timer.
	/// </summary>
	void ClearAllData() noexcept;

	/// <summary>
	/// Resets current time to 0 whether timer is valid or not.
	/// </summary>
	void ResetTime() noexcept;

	/// <summary>
	/// The timer is valid if lengthRef/length > 0.0f
	/// The callback is not required to be set for the timer to be valid.
	/// </summary>
	/// <returns></returns>
	inline constexpr bool IsValid() const noexcept { return valid; }

	/// <summary>
	/// Returns time left until the timer is completed and the onCompleted callback is executed.
	/// </summary>
	inline constexpr float GetTimeLeft() const noexcept { return lengthRef ? (*lengthRef - currentTime) : (length - currentTime); }
	inline constexpr float GetCurrentTime() const noexcept { return currentTime; }
	inline constexpr float GetCurrentLength() const noexcept { return length; }
	inline constexpr const float* GetCurrentLengthRef() const noexcept { return lengthRef; }
	inline constexpr bool GetTriggerOnce() const noexcept { return triggerOnce; }
	inline constexpr bool GetIsLocked() const noexcept { return isLocked; }

public:
	void Update(float deltaTime) noexcept;

private:
	void UpdateTime(float deltaTime) noexcept;
	void Reevaluate() noexcept;

private:
	bool valid = false;
	bool triggerOnce = false;
	bool isLocked = false;
	const float* lengthRef = nullptr;
	float length = 0.0f;
	float currentTime = 0.0f;
	Callback onCompletedCallback;
};
