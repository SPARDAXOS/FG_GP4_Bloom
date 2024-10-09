#include "GP4Testing/Systems/LevelManagement.h"


#include "Kismet/GameplayStatics.h"
#include "GP4Testing/Utility/Debugging.h"


bool ALevelManagement::LoadLevels(const TArray<FName>& keys, bool interruptOnFail, LevelsListLoadedCallback callback) noexcept {
	if (keys.Num() <= 0)
		return false;
	if (onLevelsListLoaded) { //Limitation
		Debugging::CustomWarning("Cant start loading levels list operation while one is already active! - LoadLevels");
		return false;
	}

	onLevelsListLoaded = callback; //This will lead to only 1 callback
	bool result = false;
	for (auto& entry : keys) {
		result = LoadLevel_Internal(entry, nullptr);
		if (interruptOnFail && !result) {
			onLevelsListLoaded = nullptr;
			return false;
		}
	}

	return true;
}
bool ALevelManagement::LoadLevel(const FName& key, OnOperationCompletedCallback callback) noexcept {
	return LoadLevel_Internal(key, callback);
}
bool ALevelManagement::UnloadLevels(const TArray<FName>& keys, bool interruptOnFail, LevelsListUnloadedCallback callback) noexcept {
	if (keys.Num() <= 0)
		return false;
	if (onLevelsListUnloaded) { //Limitation
		Debugging::CustomWarning("Cant start unloading levels list operation while one is already active! - UnloadLevels");
		return false;
	}

	onLevelsListUnloaded = callback;
	bool result = false;
	for (auto& entry : keys) {
		result = UnloadLevel_Internal(entry, nullptr);
		if (interruptOnFail && !result) {
			onLevelsListUnloaded = nullptr;
			return false;
		}
	}

	return true;
}
bool ALevelManagement::UnloadLevel(const FName& key, OnOperationCompletedCallback callback) noexcept {
	return UnloadLevel_Internal(key, callback);
}
bool ALevelManagement::UnloadAllLevels(OnOperationCompletedCallback callback) noexcept {
	if (currentLoadedLevels.IsEmpty())
		return false;

	return UnloadLevels(currentLoadedLevels, false, callback);
}


bool ALevelManagement::IsLevelLoaded(const FName& key) const noexcept {
	ULevelStreaming* level = FindLevel(key);
	if (!level)
		return false;

	return level->IsLevelLoaded();
}


bool ALevelManagement::LoadLevel_Internal(const FName& key, OnOperationCompletedCallback callback) noexcept {
	ULevelStreaming* level = FindLevel(key);
	if (!level) {
		Debugging::CustomError("Failed to find level associated with provided key - LoadLevel_Internal");
		return false;
	}
	if (level->IsLevelLoaded()) {
		Debugging::CustomWarning("Attempted to load an already loaded level - LoadLevel_Internal");
		return false;
	}

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.ExecutionFunction = FName("LevelLoadedCallback");
	info.Linkage = currentLinkageID;
	info.UUID = currentUUID;

	FLevelOperationSpec spec;
	spec.key = key;
	spec.linkageID = currentLinkageID;
	spec.UUID = currentUUID;
	spec.callback = callback;

	currentLinkageID++;
	currentUUID++;

	activeLoadingOperations.Add(spec);
	currentActiveLoadingOperations++;

	UGameplayStatics::LoadStreamLevel(GetWorld(), key, true, false, info);
	return true;
}
bool ALevelManagement::UnloadLevel_Internal(const FName& key, OnOperationCompletedCallback callback) noexcept {
	ULevelStreaming* level = FindLevel(key);
	if (!level) {
		Debugging::CustomError("Failed to find level associated with provided key - UnloadLevel_Internal");
		return false;
	}
	if (!level->IsLevelLoaded()) {
		Debugging::CustomWarning("Attempted to unload an already unloaded level - UnloadLevel_Internal");
		return false;
	}

	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.ExecutionFunction = FName("LevelUnloadedCallback");
	info.Linkage = currentLinkageID;
	info.UUID = currentUUID;

	FLevelOperationSpec spec;
	spec.key = key;
	spec.linkageID = currentLinkageID;
	spec.UUID = currentUUID;
	spec.callback = callback;

	currentLinkageID++;
	currentUUID++;

	activeUnloadingOperations.Add(spec);
	currentActiveUnloadingOperations++;

	UGameplayStatics::UnloadStreamLevel(GetWorld(), key, info, false);
	return true;
}
ULevelStreaming* ALevelManagement::FindLevel(const FName& key) const noexcept {
	return UGameplayStatics::GetStreamingLevel(GetWorld(), key);
}


void ALevelManagement::CleanupLoadingOperation(int32 linkage) noexcept {
	if (activeLoadingOperations.Num() <= 0) {
		Debugging::CustomWarning("CleanupLoadingOpeartion() was called while there are no active loading operations!");
		return;
	}

	int targetIndex = -1;
	for (int i = 0; i < activeLoadingOperations.Num(); i++) {
		if (activeLoadingOperations[i].linkageID == linkage) {

			currentLoadedLevels.Add(activeLoadingOperations[i].key);
			if (activeLoadingOperations[i].callback)
				activeLoadingOperations[i].callback();

			currentActiveLoadingOperations--;
			targetIndex = i;
			break;
		}
	}

	if (targetIndex != -1)
		activeLoadingOperations.RemoveAt(targetIndex); //Might crash the thing due to ue bs.
}
void ALevelManagement::CleanupUnloadingOperation(int32 linkage) noexcept {
	if (activeUnloadingOperations.Num() <= 0) {
		Debugging::CustomWarning("CleanupUnloadingOperation() was called while there were no active unloading operations!");
		return;
	}

	int targetIndex = -1;
	for (int i = 0; i < activeUnloadingOperations.Num(); i++) {
		if (activeUnloadingOperations[i].linkageID == linkage) {

			currentLoadedLevels.RemoveSingle(activeUnloadingOperations[i].key);
			if (activeUnloadingOperations[i].callback)
				activeUnloadingOperations[i].callback();

			currentActiveUnloadingOperations--;
			targetIndex = i;
			break;
		}
	}

	if (targetIndex != -1)
		activeUnloadingOperations.RemoveAt(targetIndex); //Might crash the thing due to ue bs.
}


void ALevelManagement::LevelLoadedCallback(int32 linkage) noexcept {
	CleanupLoadingOperation(linkage);
	if (currentActiveLoadingOperations) {
		if (onLevelsListLoaded) {
			onLevelsListLoaded();
			onLevelsListLoaded = nullptr;
		}
	}
}
void ALevelManagement::LevelUnloadedCallback(int32 linkage) noexcept {
	CleanupUnloadingOperation(linkage);
	if (currentActiveUnloadingOperations) {
		if (onLevelsListUnloaded) {
			onLevelsListUnloaded();
			onLevelsListUnloaded = nullptr;
		}
	}
}
