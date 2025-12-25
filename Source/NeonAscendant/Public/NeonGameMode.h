#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NeonGameMode.generated.h"

class UMissionGenerator;

UCLASS()
class NEONASCENDANT_API ANeonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANeonGameMode();

protected:
	virtual void BeginPlay() override;

public:
	// Generate a new mission
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void StartNewMission();

	UFUNCTION(BlueprintPure, Category = "Mission")
	UMissionGenerator* GetMissionGenerator() const { return MissionGenerator; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	TObjectPtr<UMissionGenerator> MissionGenerator;
};
