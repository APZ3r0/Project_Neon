#include "NeonWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ANeonWeapon::ANeonWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create weapon mesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// Create muzzle location
	MuzzleLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WeaponMesh);
}

void ANeonWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Initialize ammo based on MaxAmmo (respects Blueprint overrides)
	CurrentAmmo = MaxAmmo;
}

void ANeonWeapon::StartFire()
{
	if (bIsReloading)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	bIsFiring = true;

	if (bIsAutomatic)
	{
		// Automatic fire
		Fire();
		World->GetTimerManager().SetTimer(FireTimerHandle, this, &ANeonWeapon::Fire, FireRate, true);
	}
	else
	{
		// Semi-automatic fire
		Fire();
	}
}

void ANeonWeapon::StopFire()
{
	bIsFiring = false;

	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(FireTimerHandle);
	}
}

void ANeonWeapon::Fire()
{
	if (CurrentAmmo <= 0)
	{
		StopFire();
		return;
	}

	CurrentAmmo--;

	UWorld* World = GetWorld();
	if (!World)
		return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
		return;

	// Get camera viewpoint for accurate shooting
	FVector CameraLocation;
	FRotator CameraRotation;
	if (!GetCameraViewpoint(CameraLocation, CameraRotation))
		return;

	// Calculate shot direction from camera
	const FVector ShotDirection = CameraRotation.Vector();
	const FVector TraceStart = CameraLocation;
	const FVector TraceEnd = TraceStart + (ShotDirection * Range);

	// Perform line trace
	FHitResult HitResult;
	const bool bHit = PerformLineTrace(TraceStart, TraceEnd, HitResult);

	if (bHit)
	{
		ApplyDamageToHit(HitResult, ShotDirection, PlayerController);
	}

	// Debug visualization
	DrawDebugShot(TraceStart, bHit ? HitResult.Location : TraceEnd, bHit);
}

bool ANeonWeapon::GetCameraViewpoint(FVector& OutLocation, FRotator& OutRotation) const
{
	UWorld* World = GetWorld();
	if (!World)
		return false;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
		return false;

	PlayerController->GetPlayerViewPoint(OutLocation, OutRotation);
	return true;
}

bool ANeonWeapon::PerformLineTrace(const FVector& Start, const FVector& End, FHitResult& OutHit) const
{
	UWorld* World = GetWorld();
	if (!World)
		return false;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bTraceComplex = true;

	return World->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);
}

void ANeonWeapon::ApplyDamageToHit(const FHitResult& HitResult, const FVector& ShotDirection, APlayerController* InstigatorController)
{
	AActor* HitActor = HitResult.GetActor();
	if (HitActor)
	{
		UGameplayStatics::ApplyPointDamage(
			HitActor,
			Damage,
			ShotDirection,
			HitResult,
			InstigatorController,
			this,
			UDamageType::StaticClass()
		);
	}
}

void ANeonWeapon::DrawDebugShot(const FVector& Start, const FVector& End, bool bHit) const
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	if (bHit)
	{
		DrawDebugLine(World, Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
		DrawDebugPoint(World, End, 10.0f, FColor::Red, false, 1.0f);
	}
	else
	{
		DrawDebugLine(World, Start, End, FColor::White, false, 1.0f, 0, 1.0f);
	}
}

void ANeonWeapon::Reload()
{
	if (bIsReloading || CurrentAmmo == MaxAmmo)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	bIsReloading = true;
	StopFire();

	World->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ANeonWeapon::FinishReload, ReloadTime, false);
}

void ANeonWeapon::FinishReload()
{
	bIsReloading = false;
	CurrentAmmo = MaxAmmo;
}
