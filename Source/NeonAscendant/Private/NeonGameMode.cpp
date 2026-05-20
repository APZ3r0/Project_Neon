#include "NeonGameMode.h"
#include "NeonCharacter.h"
#include "NeonHUD.h"
#include "MissionGenerator.h"
#include "MissionTypes.h"
#include "DistrictHazard.h"
#include "NeonEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"

ANeonGameMode::ANeonGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ANeonCharacter::StaticClass();

	// Set default HUD class
	HUDClass = ANeonHUD::StaticClass();
}

void ANeonGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Get mission generator singleton
	MissionGenerator = UMissionGeneratorSingleton::GetGenerator();

	// Start the first mission immediately so enemies, hazards, and HUD are set up on level load
	StartNewMission();
}

void ANeonGameMode::StartNewMission()
{
	if (!MissionGenerator)
	{
		MissionGenerator = UMissionGeneratorSingleton::GetGenerator();
	}

	if (MissionGenerator)
	{
		FMissionBrief NewMission = MissionGenerator->GenerateMissionBrief();

		// Log mission info
		UE_LOG(LogTemp, Log, TEXT("New Mission Generated:"));
		UE_LOG(LogTemp, Log, TEXT("  District: %s"), *NewMission.District.Name);
		UE_LOG(LogTemp, Log, TEXT("  Opposition: %s"), *NewMission.Opposition.Name);
		UE_LOG(LogTemp, Log, TEXT("  Archetype: %s"), *NewMission.Archetype.Name);
		UE_LOG(LogTemp, Log, TEXT("  Weapon: %s"), *NewMission.PrimaryWeapon.Name);
		UE_LOG(LogTemp, Log, TEXT("  Complication: %s"), *NewMission.Complication);
		UE_LOG(LogTemp, Log, TEXT("  Extraction: %s"), *NewMission.ExtractionCondition);

		// Spawn enemies based on the generated mission
		SpawnEnemiesForMission(NewMission);

		// Spawn district hazards
		SpawnHazardsForMission(NewMission);

		// Update HUD with mission briefing
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			ANeonHUD* GameHUD = Cast<ANeonHUD>(PC->GetHUD());
			if (GameHUD)
			{
				GameHUD->SetMissionBrief(NewMission);

				if (APawn* PlayerPawn = PC->GetPawn())
				{
					if (ANeonCharacter* PlayerCharacter = Cast<ANeonCharacter>(PlayerPawn))
					{
						GameHUD->SetPlayerCharacter(PlayerCharacter);
					}
				}
			}

			// Load abilities onto the player character from the generated archetype
			if (APawn* PlayerPawn = PC->GetPawn())
			{
				if (ANeonCharacter* PlayerCharacter = Cast<ANeonCharacter>(PlayerPawn))
				{
					if (IsValid(PlayerCharacter))
					{
						PlayerCharacter->LoadAbilitiesFromArchetype(NewMission.Archetype);
					}
				}
			}
		}
	}
}

void ANeonGameMode::SpawnEnemiesForMission(const FMissionBrief& Mission, int32 EnemyCount)
{
	// Validate enemy class is set
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANeonGameMode::SpawnEnemiesForMission - EnemyClass not set! Assign ANeonEnemy class in Blueprint."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Get spawn origin from player start or level center
	FVector SpawnOrigin = FVector::ZeroVector;

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC && PC->GetPawn())
	{
		SpawnOrigin = PC->GetPawn()->GetActorLocation();
	}
	else
	{
		// Find player start as fallback
		APlayerStart* PlayerStart = Cast<APlayerStart>(
			UGameplayStatics::GetActorOfClass(World, APlayerStart::StaticClass())
		);
		if (PlayerStart)
		{
			SpawnOrigin = PlayerStart->GetActorLocation();
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Spawning %d enemies for mission vs %s"), EnemyCount, *Mission.Opposition.Name);

	// Use FRandomStream for determinism-compatible spawning instead of global rand.
	FRandomStream SpawnStream(FMath::Rand());

	for (int32 i = 0; i < EnemyCount; ++i)
	{
		// Calculate spawn position: random location around the spawn origin
		// In a real implementation, use designated spawn points
		FVector SpawnLocation = SpawnOrigin + FVector(
			SpawnStream.RandRange(EnemySpawnMinDistance, EnemySpawnMaxDistance),
			SpawnStream.RandRange(EnemySpawnMinDistance, EnemySpawnMaxDistance),
			EnemySpawnHeightOffset
		);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the enemy
		// Note: ANeonEnemy class has been created - see GAME_DEVELOPMENT.md Step 2
		ANeonEnemy* NewEnemy = World->SpawnActor<ANeonEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (NewEnemy)
		{
			NewEnemy->ConfigureForFaction(Mission.Opposition);
			UE_LOG(LogTemp, Log, TEXT("Spawned enemy %d at location (%.0f, %.0f, %.0f)"),
				i + 1, SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
		}
	}
}

void ANeonGameMode::SpawnHazardsForMission(const FMissionBrief& Mission)
{
	// Validate hazard class is set
	if (!HazardClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANeonGameMode::SpawnHazardsForMission - HazardClass not set! Assign ADistrictHazard class in Blueprint."));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Clear previous hazards
	for (ADistrictHazard* Hazard : ActiveHazards)
	{
		if (IsValid(Hazard))
		{
			Hazard->Destroy();
		}
	}
	ActiveHazards.Empty();

	// Use FRandomStream for determinism-compatible spawning instead of global rand.
	FRandomStream SpawnStream(FMath::Rand());

	// Spawn hazards based on the mission's complication (which relates to district)
	int32 HazardCount = SpawnStream.RandRange(MinHazardCount, MaxHazardCount);

	UE_LOG(LogTemp, Log, TEXT("Spawning %d hazards for district %s"), HazardCount, *Mission.District.Name);

	for (int32 i = 0; i < HazardCount; ++i)
	{
		// Calculate spawn position: random location around the level
		FVector SpawnLocation = FVector(
			SpawnStream.RandRange(HazardSpawnMinDistance, HazardSpawnMaxDistance),
			SpawnStream.RandRange(HazardSpawnMinDistance, HazardSpawnMaxDistance),
			HazardSpawnHeight
		);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the hazard
		ADistrictHazard* NewHazard = World->SpawnActor<ADistrictHazard>(HazardClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (NewHazard)
		{
			// Determine hazard type from district hazard strings (case-insensitive keyword match)
			EHazardType MappedType = EHazardType::Thermal; // default fallback
			if (Mission.District.Hazards.Num() > 0)
			{
				const FString HazardStr = Mission.District.Hazards[i % Mission.District.Hazards.Num()].ToLower();
				if (HazardStr.Contains(TEXT("thermal")) || HazardStr.Contains(TEXT("molten")) ||
					HazardStr.Contains(TEXT("heat"))    || HazardStr.Contains(TEXT("fire"))  ||
					HazardStr.Contains(TEXT("slag")))
				{
					MappedType = EHazardType::Thermal;
				}
				else if (HazardStr.Contains(TEXT("electric")) || HazardStr.Contains(TEXT("voltage")) ||
				         HazardStr.Contains(TEXT("shock"))    || HazardStr.Contains(TEXT("lightning")))
				{
					MappedType = EHazardType::Electrical;
				}
				else if (HazardStr.Contains(TEXT("toxic"))    || HazardStr.Contains(TEXT("poison")) ||
				         HazardStr.Contains(TEXT("acid"))     || HazardStr.Contains(TEXT("chemical")))
				{
					MappedType = EHazardType::Toxic;
				}
				else if (HazardStr.Contains(TEXT("radiation")) || HazardStr.Contains(TEXT("radioactive")) ||
				         HazardStr.Contains(TEXT("nuclear")))
				{
					MappedType = EHazardType::Radiation;
				}
				else if (HazardStr.Contains(TEXT("cryo"))  || HazardStr.Contains(TEXT("freeze")) ||
				         HazardStr.Contains(TEXT("frost")) || HazardStr.Contains(TEXT("cold"))   ||
				         HazardStr.Contains(TEXT("ice")))
				{
					MappedType = EHazardType::Cryogenic;
				}
			}
			NewHazard->HazardType = MappedType;
			NewHazard->DamagePerSecond = SpawnStream.FRandRange(5.0f, 15.0f);
			NewHazard->EffectRadius = SpawnStream.FRandRange(300.0f, 600.0f);

			ActiveHazards.Add(NewHazard);

			UE_LOG(LogTemp, Log, TEXT("Spawned hazard %d at location (%.0f, %.0f, %.0f)"), 
				i + 1, SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
		}
	}
}
