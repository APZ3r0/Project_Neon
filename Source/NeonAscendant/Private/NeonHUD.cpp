#include "NeonHUD.h"
#include "NeonCharacter.h"
#include "NeonWeapon.h"
#include "Engine/Canvas.h"

ANeonHUD::ANeonHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANeonHUD::BeginPlay()
{
	Super::BeginPlay();

	// Find player character
	for (TActorIterator<ANeonCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		PlayerCharacter = *ActorItr;
		break;
	}

	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("NeonHUD found player character"));
	}
}

void ANeonHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas)
	{
		return;
	}

	// Draw all HUD elements
	DrawHealthBar();
	DrawAmmoCounter();
	
	if (bShowMissionBriefing)
	{
		DrawMissionBriefing();
	}
	
	DrawObjectiveTracker();
	DrawExtractionIndicator();
}

void ANeonHUD::SetMissionBrief(const FMissionBrief& NewMission)
{
	CurrentMission = NewMission;
	bShowMissionBriefing = true;

	UE_LOG(LogTemp, Log, TEXT("HUD updated with mission: %s vs %s"),
		*NewMission.District.Name,
		*NewMission.Opposition.Name);
}

void ANeonHUD::SetPlayerCharacter(ANeonCharacter* NewPlayer)
{
	PlayerCharacter = NewPlayer;
}

void ANeonHUD::DrawHealthBar()
{
	if (!PlayerCharacter)
	{
		return;
	}

	FVector2D BarPosition = HealthBarPosition;
	FVector2D BarSize = HealthBarSize;

	float HealthPercent = PlayerCharacter->GetHealthPercent();

	// Draw background
	DrawFilledRect(BarPosition, BarSize, FColor::Black);

	// Draw border
	DrawBorderedRect(BarPosition, BarSize, GetHealthColor(), FColor::Black);

	// Draw health fill
	FVector2D HealthSize = FVector2D(BarSize.X * HealthPercent, BarSize.Y);
	DrawFilledRect(BarPosition + FVector2D(2.0f, 2.0f), HealthSize - FVector2D(4.0f, 4.0f), GetHealthColor());

	// Draw health text
	FString HealthText = FString::Printf(TEXT("Health: %.0f / %.0f"),
		PlayerCharacter->GetHealth(),
		PlayerCharacter->GetMaxHealth());
	
	Canvas->DrawText(
		GEngine->GetSmallFont(),
		HealthText,
		BarPosition.X + BarSize.X + 10.0f,
		BarPosition.Y,
		TextColor);
}

void ANeonHUD::DrawAmmoCounter()
{
	if (!PlayerCharacter || !PlayerCharacter->CurrentWeapon)
	{
		return;
	}

	ANeonWeapon* Weapon = PlayerCharacter->CurrentWeapon;
	FVector2D Position = AmmoCounterPosition;

	FString AmmoText = FString::Printf(TEXT("Ammo: %d / %d"),
		Weapon->GetCurrentAmmo(),
		Weapon->GetMaxAmmo());

	Canvas->DrawText(
		GEngine->GetSmallFont(),
		AmmoText,
		Position.X,
		Position.Y,
		TextColor);
}

void ANeonHUD::DrawMissionBriefing()
{
	FVector2D Position = MissionBriefingPosition;
	float LineHeight = 20.0f;
	FColor BriefingColor = FColor(0, 200, 255); // Cyan

	// Title
	Canvas->DrawText(
		GEngine->GetSmallFont(),
		TEXT("=== MISSION BRIEFING ==="),
		Position.X,
		Position.Y,
		BriefingColor);

	Position.Y += LineHeight + 10.0f;

	// Mission details
	FString DistrictText = FString::Printf(TEXT("District: %s"), *CurrentMission.District.Name);
	Canvas->DrawText(GEngine->GetSmallFont(), DistrictText, Position.X, Position.Y, TextColor);
	Position.Y += LineHeight;

	FString OppositionText = FString::Printf(TEXT("Opposition: %s"), *CurrentMission.Opposition.Name);
	Canvas->DrawText(GEngine->GetSmallFont(), OppositionText, Position.X, Position.Y, TextColor);
	Position.Y += LineHeight;

	FString ArchetypeText = FString::Printf(TEXT("Archetype: %s"), *CurrentMission.Archetype.Name);
	Canvas->DrawText(GEngine->GetSmallFont(), ArchetypeText, Position.X, Position.Y, TextColor);
	Position.Y += LineHeight;

	FString WeaponText = FString::Printf(TEXT("Primary Weapon: %s"), *CurrentMission.PrimaryWeapon.Name);
	Canvas->DrawText(GEngine->GetSmallFont(), WeaponText, Position.X, Position.Y, TextColor);
	Position.Y += LineHeight;

	FString ComplicationText = FString::Printf(TEXT("Complication: %s"), *CurrentMission.Complication);
	Canvas->DrawText(GEngine->GetSmallFont(), ComplicationText, Position.X, Position.Y, TextColor);
	Position.Y += LineHeight;

	FString ExtractionText = FString::Printf(TEXT("Extraction: %s"), *CurrentMission.ExtractionCondition);
	Canvas->DrawText(GEngine->GetSmallFont(), ExtractionText, Position.X, Position.Y, TextColor);
}

void ANeonHUD::DrawObjectiveTracker()
{
	FVector2D Position = ObjectiveTrackerPosition;
	FColor ObjectiveColor = FColor::Yellow;

	Canvas->DrawText(
		GEngine->GetSmallFont(),
		TEXT("=== OBJECTIVE ==="),
		Position.X,
		Position.Y,
		ObjectiveColor);

	Position.Y += 20.0f;

	FString ObjectiveText = FString::Printf(TEXT("Complication: %s"), *CurrentMission.Complication);
	Canvas->DrawText(GEngine->GetSmallFont(), ObjectiveText, Position.X, Position.Y, TextColor);

	Position.Y += 20.0f;

	FString ExtractionText = FString::Printf(TEXT("Extract via: %s"), *CurrentMission.ExtractionCondition);
	Canvas->DrawText(GEngine->GetSmallFont(), ExtractionText, Position.X, Position.Y, TextColor);
}

void ANeonHUD::DrawExtractionIndicator()
{
	// Draw extraction status in top right
	FString ExtractionStatus = TEXT("Extraction: ACTIVE");
	FColor ExtractionColor = FColor::Green;

	FVector2D TextSize = FVector2D::ZeroVector;
	Canvas->TextSize(GEngine->GetSmallFont(), ExtractionStatus, TextSize.X, TextSize.Y);

	FVector2D Position = FVector2D(Canvas->SizeX - TextSize.X - 20.0f, 20.0f);
	Canvas->DrawText(GEngine->GetSmallFont(), ExtractionStatus, Position.X, Position.Y, ExtractionColor);
}

FColor ANeonHUD::GetHealthColor() const
{
	if (!PlayerCharacter)
	{
		return HealthyColor;
	}

	float HealthPercent = PlayerCharacter->GetHealthPercent();

	if (HealthPercent > 0.5f)
	{
		return HealthyColor;
	}
	else if (HealthPercent > 0.25f)
	{
		return DamagedColor;
	}
	else
	{
		return CriticalColor;
	}
}

void ANeonHUD::DrawFilledRect(FVector2D Position, FVector2D Size, FColor Color)
{
	FCanvasTileItem TileItem(Position, FVector2D(1, 1), Color);
	TileItem.Size = Size;
	Canvas->DrawItem(TileItem);
}

void ANeonHUD::DrawBorderedRect(FVector2D Position, FVector2D Size, FColor BorderColor, FColor FillColor)
{
	// Draw fill
	DrawFilledRect(Position, Size, FillColor);

	// Draw border lines
	FCanvasLineItem LineItem;
	LineItem.SetColor(BorderColor);
	LineItem.LineThickness = 2.0f;

	// Top
	Canvas->DrawItem(FCanvasLineItem(Position, Position + FVector2D(Size.X, 0), BorderColor));
	// Bottom
	Canvas->DrawItem(FCanvasLineItem(Position + FVector2D(0, Size.Y), Position + Size, BorderColor));
	// Left
	Canvas->DrawItem(FCanvasLineItem(Position, Position + FVector2D(0, Size.Y), BorderColor));
	// Right
	Canvas->DrawItem(FCanvasLineItem(Position + FVector2D(Size.X, 0), Position + Size, BorderColor));
}
