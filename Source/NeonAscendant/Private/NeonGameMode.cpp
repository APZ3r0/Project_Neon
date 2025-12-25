#include "NeonGameMode.h"
#include "NeonCharacter.h"
#include "MissionGenerator.h"
#include "Kismet/GameplayStatics.h"

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
	}
}
