#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <functional>


#include "LevelManagement.generated.h"



using OnOperationCompletedCallback = std::function<void(void)>;


USTRUCT(BlueprintType)
struct FLevelOperationSpec {

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FName key;

	UPROPERTY(VisibleAnywhere)
	int32 linkageID;

	UPROPERTY(VisibleAnywhere)
	int32 UUID;

	OnOperationCompletedCallback callback;

	FLevelOperationSpec()
		: key("None"), linkageID(-1), UUID(-1), callback(nullptr)
	{

	}

};



UCLASS()
class ALevelManagement : public AActor {

	GENERATED_BODY()

public:
	using LevelsListLoadedCallback = OnOperationCompletedCallback;
	using LevelsListUnloadedCallback = OnOperationCompletedCallback;

public:
	bool LoadLevels(const TArray<FName>& keys, bool interruptOnFail, LevelsListLoadedCallback callback = nullptr) noexcept;
	bool LoadLevel(const FName& key, OnOperationCompletedCallback callback = nullptr) noexcept;
	bool UnloadLevels(const TArray<FName>& keys, bool interruptOnFail, LevelsListUnloadedCallback callback = nullptr) noexcept;
	bool UnloadLevel(const FName& key, OnOperationCompletedCallback callback = nullptr) noexcept;
	bool UnloadAllLevels(OnOperationCompletedCallback callback = nullptr) noexcept;

public:
	bool IsLevelLoaded(const FName& key) const noexcept;

private:
	bool LoadLevel_Internal(const FName& key, OnOperationCompletedCallback callback = nullptr) noexcept;
	bool UnloadLevel_Internal(const FName& key, OnOperationCompletedCallback callback = nullptr) noexcept;
	ULevelStreaming* FindLevel(const FName& key) const noexcept;

private:
	void CleanupLoadingOperation(int32 linkage) noexcept;
	void CleanupUnloadingOperation(int32 linkage) noexcept;

private:
	UFUNCTION()
	void LevelLoadedCallback(int32 linkage) noexcept;

	UFUNCTION()
	void LevelUnloadedCallback(int32 linkage) noexcept;

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<FLevelOperationSpec> activeLoadingOperations;

	UPROPERTY(VisibleAnywhere)
	TArray<FLevelOperationSpec> activeUnloadingOperations;

	UPROPERTY(VisibleAnywhere)
	TArray<FName> currentLoadedLevels;

private:
	LevelsListLoadedCallback onLevelsListLoaded;
	LevelsListUnloadedCallback onLevelsListUnloaded;

private:
	int32 currentUUID = 0;
	int32 currentLinkageID = 0;
	int32 currentActiveLoadingOperations = 0;
	int32 currentActiveUnloadingOperations = 0;
};