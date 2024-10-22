
#include "PrimaryPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GP4Testing/Systems/PrimaryGameMode.h"
#include "GP4Testing/PlayerSystems/PlayerMovementSystem.h"
#include "GP4Testing/PlayerSystems/PlayerHealthSystem.h"
#include "GP4Testing/PlayerSystems/PickupManagementSystem.h"
#include "GP4Testing/PlayerSystems/WeaponManagementSystem.h"
#include "GP4Testing/GUI/PrimaryPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "GP4Testing/AI/EnemyAIBase.h"

#include "GP4Testing/Utility/Debugging.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


APrimaryPlayer::APrimaryPlayer() {
	PrimaryActorTick.bCanEverTick = true;

	SetupCamera();
}



//Use these instead!
void APrimaryPlayer::Init() {
	CreatePlayerSystems();
	SetupPlayerSystemsDependencies();
	InitPlayerSystems();
	SetupPrimaryPlayerHUD();
	//Delegates to handle camera shake
	GetPlayerHealthSystem().HealthComponent->OnDamage.AddUniqueDynamic(this, &APrimaryPlayer::HandleHitShake);
	CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);

	springArm->TargetArmLength = springArmLength;
	springArm->SetRelativeTransform(cameraInitialTransform);
}
void APrimaryPlayer::Start() {
	StartPlayerSystems();
}
void APrimaryPlayer::Update(float deltaTime) {
	if(GetCharacterMovement()->GetLastUpdateVelocity().Length() > 0 && bIsGrounded) // same thing as getting the ABS
	{
		HandleRunningShake();
	}
	/*
	else if (GetCharacterMovement()->GetLastUpdateVelocity().Length() == 0)
	{
		StopShakeCamera(); 
	}
	*/
}
void APrimaryPlayer::SetupStartingState() noexcept {
	//Reset all player data to default.
	if (playerMovementSystemRef)
		playerMovementSystemRef->SetupStartingState();

	if (playerHealthSystemRef)
		playerHealthSystemRef->SetupStartingState();

	if (weaponManagementSystemRef)
		weaponManagementSystemRef->SetupStartingState();
}
void APrimaryPlayer::SetPlayerState(bool state) noexcept {
	SetActorTickEnabled(state);
	SetActorEnableCollision(state);
	SetActorHiddenInGame(!state);

	GetCapsuleComponent()->SetEnableGravity(state);
	GetCharacterMovement()->SetActive(state);
	if (state)
		GetCharacterMovement()->GravityScale =	1.0f;	//defaultGravityScale;
	else
		GetCharacterMovement()->GravityScale = 0.0f;

	active = state;
}
void APrimaryPlayer::SetPlayerHUDState(bool state) noexcept {
	if (state)
		primaryPlayerHUDRef->SetVisibilityState(ESlateVisibility::Visible);
	else
		primaryPlayerHUDRef->SetVisibilityState(ESlateVisibility::Collapsed);
}
AWeaponManagementSystem* APrimaryPlayer::GetWeaponManagementSystemPointer()
{
	return weaponManagementSystemRef;
}
//HandleInput
void APrimaryPlayer::HandleMovementInput(FVector2D axis) noexcept {
	if (!playerMovementSystemRef)
		return;

	playerMovementSystemRef->UpdateMovement(axis);
}
void APrimaryPlayer::HandleLookInput(FVector2D axis) noexcept {
	if (!playerMovementSystemRef)
		return;

	playerMovementSystemRef->UpdateRotation(axis);
}
void APrimaryPlayer::HandleJumpInput() noexcept {
	if (!playerMovementSystemRef)
		return;

	playerMovementSystemRef->Jump();
}
void APrimaryPlayer::HandleDashInput() noexcept {
	if (!playerMovementSystemRef)
		return;

	playerMovementSystemRef->Dash();
}
void APrimaryPlayer::HandleSlideInput(bool& input) noexcept {
	if (!playerMovementSystemRef)
		return;

	playerMovementSystemRef->Slide(input);
}
void APrimaryPlayer::HandleShootInput(bool& input) noexcept {
	//input == true => Pressed
	//input == false => Released
	if (!weaponManagementSystemRef)
		return;

	weaponManagementSystemRef->UseCurrentWeapon(input);
}
void APrimaryPlayer::HandleMeleeInput() noexcept {
	if (bCanMelee)
	{
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, GetCamera()->GetComponentLocation(), (GetCamera()->GetForwardVector()*MeleeRange)+GetCamera()->GetComponentLocation(), ECC_GameTraceChannel3);
		DrawDebugLine(GetWorld(), GetCamera()->GetComponentLocation(), (GetCamera()->GetForwardVector()*MeleeRange)+GetCamera()->GetComponentLocation(), FColor::Black, false, 3);
		if (Hit.bBlockingHit)
		{
			AEnemyAIBase* HitEnemy = Cast<AEnemyAIBase>(Hit.GetActor());
			if(HitEnemy)
			{
				HitEnemy->HealthComponent->TakeDamage(40);
			}
		}
		bCanMelee = false;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APrimaryPlayer::ResetMelee, MeleeCooldown);
	}
}
void APrimaryPlayer::ResetMelee()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	bCanMelee = true;
}
void APrimaryPlayer::HandlePauseInput() noexcept {
	if (!primaryGameModeRef->GetGamePaused())
		primaryGameModeRef->PauseGame();
	else
		primaryGameModeRef->UnpauseGame();
}
void APrimaryPlayer::HandleReloadInput() noexcept {
	if (!weaponManagementSystemRef)
		return;

	weaponManagementSystemRef->ReloadWeapon();
}
void APrimaryPlayer::HandleSwitchNextWeaponInput() noexcept {
	if (!weaponManagementSystemRef)
		return;

	weaponManagementSystemRef->SwitchNextWeapon();
}
void APrimaryPlayer::HandleSwitchPreviousWeaponInput() noexcept {
	if (!weaponManagementSystemRef)
		return;

	weaponManagementSystemRef->SwitchPreviousWeapon();
}
void APrimaryPlayer::HandleWeaponSlot1Input() noexcept {
	if (!weaponManagementSystemRef)
		return;

	weaponManagementSystemRef->WeaponSlot1();
}
void APrimaryPlayer::HandleWeaponSlot2Input() noexcept {
	if (!weaponManagementSystemRef)
		return;

	weaponManagementSystemRef->WeaponSlot2();
}
void APrimaryPlayer::HandleWeaponSlot3Input() noexcept {
	if (!weaponManagementSystemRef)
		return;
	

	weaponManagementSystemRef->WeaponSlot3();
}
void APrimaryPlayer::ShakeCamera(TSubclassOf<UCameraShakeBase> CameraShakeBase, float Scale)
{
	if (CameraManager)
	{
		CameraManager->StartCameraShake(CameraShakeBase, Scale);
	}
}
void APrimaryPlayer::HandleHitShake()
{
	ShakeCamera(HitShake, 1);
}
void APrimaryPlayer::OnJumped_Implementation()
{
	bIsGrounded = false;
	StartJumpDistance = GetCharacterMovement()->GetActorLocation().Z;
	Debugging::PrintString("Jumped");
	playerMovementSystemRef->PlayJumpAudio();
}
void APrimaryPlayer::Landed(const FHitResult& Hit) // need to convert to a fall length check
{
	Super::Landed(Hit);
	bIsGrounded = true;
	DistanceFallen = StartJumpDistance - GetCharacterMovement()->GetActorLocation().Z;
	Debugging::PrintString(FString::SanitizeFloat(DistanceFallen));
	if (DistanceFallen >= 150)
	{
		UGameplayStatics::PlaySoundAtLocation(this, fallLandSound, GetActorLocation());
		float Strength = DistanceFallen / MaxFallHeight;
		ShakeCamera(LandShake, Strength);
	}
}
void APrimaryPlayer::HandleShootShakeRifle()
{
	ShakeCamera(ShootingShakeRifle, 1.f);
}
void APrimaryPlayer::HandleShootShakeShotgun()
{
	ShakeCamera(ShootingShakeShotgun, 1.f);
}
void APrimaryPlayer::HandleRunningShake()
{
	float Strength = GetCharacterMovement()->GetLastUpdateVelocity().Length() / MaxRunSpeed;
	ShakeCamera(RunningShake, Strength);
}
void APrimaryPlayer::CreatePlayerSystems() {
	checkf(playerMovementSystemAsset, TEXT("PlayerMovementSystemAsset ref is null!"));
	playerMovementSystemRef = GetWorld()->SpawnActor<APlayerMovementSystem>(playerMovementSystemAsset);
	checkf(playerMovementSystemRef, TEXT("Failed to create PlayerMovementSystem!"));

	checkf(playerHealthSystemAsset, TEXT("PlayerHealthSystemAsset ref is null!"));
	playerHealthSystemRef = GetWorld()->SpawnActor<APlayerHealthSystem>(playerHealthSystemAsset);
	checkf(playerHealthSystemRef, TEXT("Failed to create PlayerHealthSystem!"));

	checkf(weaponManagementSystemAsset, TEXT("WeaponManagementSystemAsset ref is null!"));
	weaponManagementSystemRef = GetWorld()->SpawnActor<AWeaponManagementSystem>(weaponManagementSystemAsset);
	checkf(weaponManagementSystemRef, TEXT("Failed to create WeaponManagementSystem!"));

	checkf(pickupManagementSystemAsset, TEXT("PickupManagementSystemAsset ref is null!"));
	pickupManagementSystemRef = GetWorld()->SpawnActor<APickupManagementSystem>(pickupManagementSystemAsset);
	checkf(pickupManagementSystemRef, TEXT("Failed to create PickupManagementSystem!"));
}
void APrimaryPlayer::SetupPlayerSystemsDependencies() noexcept {
	if (playerMovementSystemRef) {
		playerMovementSystemRef->RegisterPrimaryPlayerReference(*this);

	}

	if (playerHealthSystemRef) {
		playerHealthSystemRef->RegisterPrimaryPlayerReference(*this);

	}

	if (weaponManagementSystemRef) {
		weaponManagementSystemRef->RegisterPrimaryPlayerReference(*this);

	}

	if (pickupManagementSystemRef) {
		pickupManagementSystemRef->RegisterPrimaryPlayerReference(*this);

	}
}
void APrimaryPlayer::InitPlayerSystems() noexcept {
	//if (playerMovementSystemRef)
	//	playerMovementSystemRef->Init();
	//
	//if (playerHealthSystemRef)
	//	playerHealthSystemRef->Init();
	//
	//if (weaponManagementSystemRef)
	//	weaponManagementSystemRef->Init();
	//
	//if (pickupManagementSystemRef)
	//	pickupManagementSystemRef->Init();
}
void APrimaryPlayer::StartPlayerSystems() noexcept {
	//if (playerMovementSystemRef)
	//	playerMovementSystemRef->Start();
	//
	//if (playerHealthSystemRef)
	//	playerHealthSystemRef->Start();
	//
	//if (weaponManagementSystemRef)
	//	weaponManagementSystemRef->Start();
	//
	//if (pickupManagementSystemRef)
	//	pickupManagementSystemRef->Start();
}
void APrimaryPlayer::SetupCamera() noexcept {
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetCapsuleComponent());
	springArm->bUsePawnControlRotation = true;
	
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	cameraComponent->SetupAttachment(springArm);
	GetMesh()->SetupAttachment(cameraComponent);
}
void APrimaryPlayer::SetupPrimaryPlayerHUD() noexcept {
	checkf(primaryPlayerHUDClass, TEXT("primaryPlayerHUDClass ref is null!"));
	primaryPlayerHUDRef = CreateWidget<UPrimaryPlayerHUD>(GetWorld(), primaryPlayerHUDClass);
	checkf(primaryPlayerHUDRef, TEXT("Failed to create primaryPlayerHUDRef!"));

	primaryPlayerHUDRef->AddToViewport();
	primaryPlayerHUDRef->SetPrimaryPlayerReference(*this);
	SetPlayerHUDState(false);
}
