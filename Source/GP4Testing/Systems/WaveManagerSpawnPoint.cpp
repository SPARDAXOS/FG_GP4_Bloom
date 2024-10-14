#include "GP4Testing/Systems/WaveManagerSpawnPoint.h"
#include "Components/BillboardComponent.h"




AWaveManagerSpawnPoint::AWaveManagerSpawnPoint() {

	root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	tag = CreateDefaultSubobject<UBillboardComponent>("Tag");
	tag->SetupAttachment(root);
}