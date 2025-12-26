#include "NeonGameMode.h"
#include "NeonCharacter.h"
#include "MissionGenerator.h"
#include "MissionTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ANeonGameMode::ANeonGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ANeonCharacter::StaticClass();
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

	// Gather spawn points from the level (look for actors with "EnemySpawn" tag or similar)
	// For now, we'll use random positions around the player spawn
	APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass);
	FVector PlayerLocation = PlayerPawn ? PlayerPawn->GetActorLocation() : FVector::ZeroVector;

	UE_LOG(LogTemp, Log, TEXT("Spawning %d enemies for mission vs %s"), EnemyCount, *Mission.Opposition.Name);

	for (int32 i = 0; i < EnemyCount; ++i)
	{
		// Calculate spawn position: random location around the level
		// In a real implementation, use designated spawn points
		FVector SpawnLocation = PlayerLocation + FVector(
			FMath::RandRange(-2000.0f, 2000.0f),
			FMath::RandRange(-2000.0f, 2000.0f),
			0.0f
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
