#pragma once


class Entity {

public:
	/// <summary>
	/// Called during game initialization and is the earliest point in the game's initialization process.
	/// </summary>
	inline virtual void Init() {}

	/// <summary>
	/// Called at the start of the game after MainGameMode signals BeginPlay().
	/// </summary>
	inline virtual void Start() {}

	/// <summary>
	/// Called instead of Tick() by the MainGameMode.
	/// </summary>
	/// <param name="deltaTime"></param>
	inline virtual void Update(float deltaTime) {}
};