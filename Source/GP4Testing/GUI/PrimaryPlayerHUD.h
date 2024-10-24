#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GP4Testing/Systems/Entity.h"
#include "PrimaryPlayerHUD.generated.h"



class APrimaryPlayer;
class APrimaryHUD;

class UCanvasPanel;
class UProgressBar;
class UTextBlock;
class UImage;

UCLASS(Abstract)
class GP4TESTING_API UPrimaryPlayerHUD : public UUserWidget, public Entity {
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

public:
	virtual void SetVisibilityState(ESlateVisibility state) noexcept;
	virtual void SetWidgetOpacity(float value) noexcept;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	inline void SetPrimaryPlayerReference(APrimaryPlayer& reference) noexcept { primaryPlayerRef = &reference; }
	inline ESlateVisibility GetVisibility() const noexcept { return visible; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> mainCanvas = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Crosshair = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveCounter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> DashAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SlideAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DashKey = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SlideKey = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> EnemiesLeftText = nullptr;

	void HandleHealthBar();
	void HandleAmmoText();
	void HandleWaveCounter();
	void HandleDashCooldown();
	void HandleSlideCooldown();
	void HandleEnemiesLeftText();

protected:
	ESlateVisibility visible;

protected:
	APrimaryPlayer* primaryPlayerRef = nullptr;
};