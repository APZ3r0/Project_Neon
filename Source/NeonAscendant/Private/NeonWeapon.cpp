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

	bIsFiring = true;

	if (bIsAutomatic)
	{
		// Automatic fire
		Fire();
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ANeonWeapon::Fire, FireRate, true);
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
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void ANeonWeapon::Fire()
{
	if (CurrentAmmo <= 0)
	{
		StopFire();
		return;
	}

	CurrentAmmo--;

	// Get camera viewpoint for accurate shooting
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
		return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// Calculate shot direction from camera
	FVector ShotDirection = CameraRotation.Vector();
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (ShotDirection * Range);

	// Perform line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bTraceComplex = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		// Apply damage to hit actor
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UGameplayStatics::ApplyPointDamage(
				HitActor,
				Damage,
				ShotDirection,
				HitResult,
				PlayerController,
				this,
				UDamageType::StaticClass()
			);
		}

		// Debug visualization
		DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 1.0f, 0, 2.0f);
		DrawDebugPoint(GetWorld(), HitResult.Location, 10.0f, FColor::Red, false, 1.0f);
	}
	else
	{
		// Debug visualization for miss
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
	}
}

void ANeonWeapon::Reload()
{
	if (bIsReloading || CurrentAmmo == MaxAmmo)
		return;

	bIsReloading = true;
	StopFire();

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ANeonWeapon::FinishReload, ReloadTime, false);
}

void ANeonWeapon::FinishReload()
{
	bIsReloading = false;
	CurrentAmmo = MaxAmmo;
}
