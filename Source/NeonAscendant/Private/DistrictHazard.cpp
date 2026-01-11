#include "DistrictHazard.h"
#include "NeonCharacter.h"
#include "NeonEnemy.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

ADistrictHazard::ADistrictHazard()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

	// Create hazard volume
	HazardVolume = CreateDefaultSubobject<USphereComponent>(TEXT("HazardVolume"));
	RootComponent = HazardVolume;
	HazardVolume->SetSphereRadius(EffectRadius);
	HazardVolume->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HazardVolume->SetCollisionObjectType(ECC_WorldDynamic);
	HazardVolume->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Create particle effect
	HazardEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HazardEffect"));
	HazardEffect->SetupAttachment(RootComponent);
}

void ADistrictHazard::BeginPlay()
{
	Super::BeginPlay();

	// Bind overlap events
	HazardVolume->OnComponentBeginOverlap.AddDynamic(this, &ADistrictHazard::OnHazardBeginOverlap);
	HazardVolume->OnComponentEndOverlap.AddDynamic(this, &ADistrictHazard::OnHazardEndOverlap);

	// Setup visual effects
	CreateHazardEffects();

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Log, TEXT("DistrictHazard spawned: %s at %s with radius %.0f"),
		*GetHazardTypeName(),
		*GetActorLocation().ToString(),
		EffectRadius);
#endif
}

void ADistrictHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsActive)
	{
		return;
	}

	// Apply damage to overlapping actors
	TArray<AActor*> OverlappingActors;
	HazardVolume->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor)
		{
			continue;
		}

		// Check if actor is a character or enemy - damage both
		if (Actor->IsA<ANeonCharacter>() || Actor->IsA<ANeonEnemy>())
		{
			ApplyHazardDamage(Actor);
		}
	}
}

void ADistrictHazard::OnHazardBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || !bIsActive)
	{
		return;
	}

	// Only interact with characters
	if (!Cast<ANeonCharacter>(OtherActor) && !Cast<ANeonEnemy>(OtherActor))
	{
		return;
	}

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Log, TEXT("Actor %s entered %s hazard"),
		*OtherActor->GetName(),
		*GetHazardTypeName());
#endif
}

void ADistrictHazard::OnHazardEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!OtherActor)
	{
		return;
	}

	// Remove damage tracking for this actor
	LastDamageTime.Remove(OtherActor);

#if !UE_BUILD_SHIPPING
	UE_LOG(LogTemp, Log, TEXT("Actor %s left %s hazard"),
		*OtherActor->GetName(),
		*GetHazardTypeName());
#endif
}

void ADistrictHazard::ApplyHazardDamage(AActor* HitActor)
{
	if (!HitActor)
	{
		return;
	}

	// Check if enough time has passed since last damage
	double CurrentTime = GetWorld()->GetTimeSeconds();
	double* LastDamagePtr = LastDamageTime.Find(HitActor);
	double TimeSinceDamage = LastDamagePtr ? (CurrentTime - *LastDamagePtr) : DamageTickRate;

	if (TimeSinceDamage < DamageTickRate)
	{
		return;
	}

	// Update last damage time
	LastDamageTime.Add(HitActor, CurrentTime);

	// Calculate damage
	float Damage = DamagePerSecond * DamageTickRate;

	// Apply damage to player
	if (ANeonCharacter* PlayerCharacter = Cast<ANeonCharacter>(HitActor))
	{
		FDamageEvent DamageEvent;
		PlayerCharacter->TakeDamage(Damage, DamageEvent, nullptr, this);

#if !UE_BUILD_SHIPPING
		UE_LOG(LogTemp, Log, TEXT("%s hazard dealt %.0f damage to player. Health: %.0f/%.0f"),
			*GetHazardTypeName(),
			Damage,
			PlayerCharacter->GetHealth(),
			PlayerCharacter->GetMaxHealth());
#endif
	}

	// Apply damage to enemy
	if (ANeonEnemy* Enemy = Cast<ANeonEnemy>(HitActor))
	{
		FDamageEvent DamageEvent;
		Enemy->TakeDamage(Damage, DamageEvent, nullptr, this);

#if !UE_BUILD_SHIPPING
		UE_LOG(LogTemp, Log, TEXT("%s hazard dealt %.0f damage to enemy. Health: %.0f/%.0f"),
			*GetHazardTypeName(),
			Damage,
			Enemy->CurrentHealth,
			Enemy->MaxHealth);
#endif
	}
}

void ADistrictHazard::CreateHazardEffects()
{
	// TODO: Link to particle systems based on hazard type
	// For now, just set up the visual component

	if (HazardEffect)
	{
		// Different colors for different hazards
		switch (HazardType)
		{
			case EHazardType::Thermal:
				HazardColor = FColor::Red;
				break;
			case EHazardType::Electrical:
				HazardColor = FColor::Yellow;
				break;
			case EHazardType::Toxic:
				HazardColor = FColor::Green;
				break;
			case EHazardType::Radiation:
				HazardColor = FColor::Cyan;
				break;
			case EHazardType::Cryogenic:
				HazardColor = FColor::Blue;
				break;
		}
	}
}

FString ADistrictHazard::GetHazardTypeName() const
{
	return UEnum::GetValueAsString(HazardType);
}
