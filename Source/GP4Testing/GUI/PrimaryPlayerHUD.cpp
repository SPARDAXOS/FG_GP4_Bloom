#include "PrimaryPlayerHUD.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "CustomButton.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GP4Testing/PlayerSystems/WeaponManagementSystem.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"


void UPrimaryPlayerHUD::NativeOnInitialized() {
	UUserWidget::NativeOnInitialized();


}


//Dont touch those unless you have a good reason to.
void UPrimaryPlayerHUD::SetVisibilityState(ESlateVisibility state) noexcept {
	visible = state;
	SetVisibility(visible);
}
void UPrimaryPlayerHUD::SetWidgetOpacity(float value) noexcept {
	SetRenderOpacity(value);
}

void UPrimaryPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//Healthbar
	float PercentToShow = (primaryPlayerRef->GetPlayerHealthSystem().HealthComponent->CurrentHealth / primaryPlayerRef->GetPlayerHealthSystem().HealthComponent->MaxHealth);
	HealthBar->SetPercent(PercentToShow);
	//Ammo counter
	AmmoText->Text = primaryPlayerRef->GetWeaponManagementSystem().GetCurrentWeapon()->Ammo
}



