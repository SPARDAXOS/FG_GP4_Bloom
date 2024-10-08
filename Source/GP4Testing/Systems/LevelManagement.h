#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <functional>


#include "LevelManagement.generated.h"



using OnOperationCompletedCallback = std::function<void>(void);


USTRUCT(BlueprintType)
struct FLevelOperationSpec {

	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FName name;

	UPROPERTY(VisibleAnywhere)
	int32 linkageID;

	UPROPERTY(VisibleAnywhere)
	int32 UUID;

	OnOperationCompletedCallback callback;

	FLevelOperationSpec() 
		:	name("None"), linkageID(-1), 
	{

	}

}



UCLASS()
class ALevelManagement : public AActor {

	GENERATED_BODY()

public:


};