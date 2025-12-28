#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "MissionTypes.h"
#include "NeonHUD.generated.h"

class ANeonCharacter;
class UMissionGenerator;

UCLASS()
class NEONASCENDANT_API ANeonHUD : public AHUD
{
	GENERATED_BODY()

public:
	ANeonHUD();

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	// Set mission brief to display
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetMissionBrief(const FMissionBrief& NewMission);

	// Update player reference
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetPlayerCharacter(ANeonCharacter* NewPlayer);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	FMissionBrief CurrentMission;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	ANeonCharacter* PlayerCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	bool bShowMissionBriefing = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
	FColor HealthyColor = FColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
	FColor DamagedColor = FColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
	FColor CriticalColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Colors")
	FColor TextColor = FColor::White;

	// UI Positions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Layout")
	FVector2D HealthBarPosition = FVector2D(20.0f, 20.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Layout")
	FVector2D HealthBarSize = FVector2D(300.0f, 30.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Layout")
	FVector2D MissionBriefingPosition = FVector2D(20.0f, 70.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Layout")
	FVector2D ObjectiveTrackerPosition = FVector2D(20.0f, 400.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|Layout")
	FVector2D AmmoCounterPosition = FVector2D(20.0f, 100.0f);

	// Draw functions
	void DrawHealthBar();
	void DrawMissionBriefing();
	void DrawObjectiveTracker();
	void DrawAmmoCounter();
	void DrawExtractionIndicator();

	// Helper functions
	FColor GetHealthColor() const;
	void DrawFilledRect(FVector2D Position, FVector2D Size, FColor Color);
	void DrawBorderedRect(FVector2D Position, FVector2D Size, FColor BorderColor, FColor FillColor);
};
