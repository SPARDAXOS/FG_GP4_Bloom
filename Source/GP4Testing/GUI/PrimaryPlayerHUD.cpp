#include "PrimaryPlayerHUD.h"

#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CustomButton.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GP4Testing/PlayerSystems/WeaponManagementSystem.h"
#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/Systems/PrimaryPlayer.h"
#include "GP4Testing/PlayerSystems/PlayerMovementSystem.h"
#include "GP4Testing/Systems/WaveManager.h"
#include "GP4Testing/Utility/Debugging.h"
#include "Kismet/KismetMathLibrary.h"


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
	HandleHealthBar();
	HandleAmmoText();
	HandleWaveCounter();
	HandleDashCooldown();
	HandleSlideCooldown();
	HandleEnemiesLeftText();
}

void UPrimaryPlayerHUD::HandleHealthBar()
{
	float PercentToShow = (primaryPlayerRef->GetPlayerHealthSystem().HealthComponent->CurrentHealth / primaryPlayerRef->GetPlayerHealthSystem().HealthComponent->MaxHealth);
	HealthBar->SetPercent(PercentToShow);
}
void UPrimaryPlayerHUD::HandleAmmoText()
{
	if (primaryPlayerRef->GetWeaponManagementSystem().GetCurrentWeapon())
	{
		FString CurrentAmmoString = FString::FromInt(UKismetMathLibrary::Round(primaryPlayerRef->GetWeaponManagementSystem().GetCurrentWeapon()->Magazine));
		FString TotalAmmoString = FString::FromInt(UKismetMathLibrary::Round(primaryPlayerRef->GetWeaponManagementSystem().GetCurrentWeapon()->Ammo));
		FText TextToDisplay = FText::FromString(CurrentAmmoString+"/"+TotalAmmoString);
		AmmoText->SetText(TextToDisplay);
	}
	else
	{
		FText Text = FText::FromString("");
		AmmoText->SetText(Text);
	}
}
void UPrimaryPlayerHUD::HandleWaveCounter()
{
	if (primaryPlayerRef->GetPrimaryGameMode()->GetWaveManager()->GetMaxWaveCount() > 0)
	{
		FString CurrentWave = FString::FromInt(primaryPlayerRef->GetPrimaryGameMode()->GetWaveManager()->GetCurrentWaveIndex() + 1);
		FString MaxWaves = FString::FromInt(primaryPlayerRef->GetPrimaryGameMode()->GetWaveManager()->GetMaxWaveCount());
		WaveCounter->SetText(FText::FromString(CurrentWave+"/"+MaxWaves));
	}
	else
	{
		WaveCounter->SetText(FText::FromString(""));
	}
}

void UPrimaryPlayerHUD::HandleDashCooldown()
{
	if (primaryPlayerRef->GetPlayerMovementSystem().GetCanDash())
	{
		DashAbility->SetColorAndOpacity(FColor::FromHex("FFFFFFFF"));
	}
	else
	{
		DashAbility->SetColorAndOpacity(FColor::FromHex("1B1B1BFF"));
	}
}

void UPrimaryPlayerHUD::HandleSlideCooldown()
{
	if (primaryPlayerRef->GetPlayerMovementSystem().GetCanSlide())
	{
		SlideAbility->SetColorAndOpacity(FColor::FromHex("FFFFFFFF"));
	}
	else
	{
		SlideAbility->SetColorAndOpacity(FColor::FromHex("1B1B1BFF"));
	}
}

void UPrimaryPlayerHUD::HandleEnemiesLeftText()
{
	int integer = primaryPlayerRef->GetPrimaryGameMode()->GetWaveManager()->GetCurrentSpawnedEnemiesCount();
	FString string = "Enemies Left: " + FString::FromInt(integer);
	EnemiesLeftText->SetText(FText::FromString(string));
}



