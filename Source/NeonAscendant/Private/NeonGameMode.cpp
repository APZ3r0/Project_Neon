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

	for (int32 i = 0; i < EnemyCount; ++i)
	{
		// Calculate spawn position: random location around the spawn origin
		// In a real implementation, use designated spawn points
		FVector SpawnLocation = SpawnOrigin + FVector(
			FMath::RandRange(EnemySpawnMinDistance, EnemySpawnMaxDistance),
			FMath::RandRange(EnemySpawnMinDistance, EnemySpawnMaxDistance),
			EnemySpawnHeightOffset
		);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the enemy
		// Note: ANeonEnemy class has been created - see GAME_DEVELOPMENT.md Step 2
		ANeonEnemy* NewEnemy = World->SpawnActor<ANeonEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (NewEnemy)
		{
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
		if (Hazard)
		{
			Hazard->Destroy();
		}
	}
	ActiveHazards.Empty();

	// Spawn hazards based on the mission's complication (which relates to district)
	int32 HazardCount = FMath::RandRange(MinHazardCount, MaxHazardCount);

	UE_LOG(LogTemp, Log, TEXT("Spawning %d hazards for district %s"), HazardCount, *Mission.District.Name);

	for (int32 i = 0; i < HazardCount; ++i)
	{
		// Calculate spawn position: random location around the level
		FVector SpawnLocation = FVector(
			FMath::RandRange(HazardSpawnMinDistance, HazardSpawnMaxDistance),
			FMath::RandRange(HazardSpawnMinDistance, HazardSpawnMaxDistance),
			HazardSpawnHeight
		);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the hazard
		ADistrictHazard* NewHazard = World->SpawnActor<ADistrictHazard>(HazardClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		if (NewHazard)
		{
			// Randomize hazard type
			int32 HazardTypeIndex = FMath::RandRange(0, 4);
			NewHazard->HazardType = static_cast<EHazardType>(HazardTypeIndex);
			NewHazard->DamagePerSecond = FMath::RandRange(5.0f, 15.0f);
			NewHazard->EffectRadius = FMath::RandRange(300.0f, 600.0f);

			ActiveHazards.Add(NewHazard);

			UE_LOG(LogTemp, Log, TEXT("Spawned hazard %d at location (%.0f, %.0f, %.0f)"), 
				i + 1, SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
		}
	}
}
