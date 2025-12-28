#include "NeonHUD.h"
#include "NeonCharacter.h"
#include "NeonWeapon.h"
#include "Engine/Canvas.h"
#include "EngineUtils.h"
#include "CanvasItem.h"

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

	FCanvasTextItem TextItem(FVector2D(BarPosition.X + BarSize.X + 10.0f, BarPosition.Y), FText::FromString(HealthText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(TextItem);
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

	FCanvasTextItem TextItem(Position, FText::FromString(AmmoText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(TextItem);
}

void ANeonHUD::DrawMissionBriefing()
{
	FVector2D Position = MissionBriefingPosition;
	float LineHeight = 20.0f;
	FColor BriefingColor = FColor(0, 200, 255); // Cyan

	// Title
	FCanvasTextItem TitleItem(Position, FText::FromString(TEXT("=== MISSION BRIEFING ===")), GEngine->GetSmallFont(), FLinearColor(BriefingColor));
	Canvas->DrawItem(TitleItem);

	Position.Y += LineHeight + 10.0f;

	// Mission details
	FString DistrictText = FString::Printf(TEXT("District: %s"), *CurrentMission.District.Name);
	FCanvasTextItem DistrictItem(Position, FText::FromString(DistrictText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(DistrictItem);
	Position.Y += LineHeight;

	FString OppositionText = FString::Printf(TEXT("Opposition: %s"), *CurrentMission.Opposition.Name);
	FCanvasTextItem OppositionItem(Position, FText::FromString(OppositionText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(OppositionItem);
	Position.Y += LineHeight;

	FString ArchetypeText = FString::Printf(TEXT("Archetype: %s"), *CurrentMission.Archetype.Name);
	FCanvasTextItem ArchetypeItem(Position, FText::FromString(ArchetypeText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(ArchetypeItem);
	Position.Y += LineHeight;

	FString WeaponText = FString::Printf(TEXT("Primary Weapon: %s"), *CurrentMission.PrimaryWeapon.Name);
	FCanvasTextItem WeaponItem(Position, FText::FromString(WeaponText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(WeaponItem);
	Position.Y += LineHeight;

	FString ComplicationText = FString::Printf(TEXT("Complication: %s"), *CurrentMission.Complication);
	FCanvasTextItem ComplicationItem(Position, FText::FromString(ComplicationText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(ComplicationItem);
	Position.Y += LineHeight;

	FString ExtractionText = FString::Printf(TEXT("Extraction: %s"), *CurrentMission.ExtractionCondition);
	FCanvasTextItem ExtractionItem(Position, FText::FromString(ExtractionText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(ExtractionItem);
}

void ANeonHUD::DrawObjectiveTracker()
{
	FVector2D Position = ObjectiveTrackerPosition;
	FColor ObjectiveColor = FColor::Yellow;

	FCanvasTextItem TitleItem(Position, FText::FromString(TEXT("=== OBJECTIVE ===")), GEngine->GetSmallFont(), FLinearColor(ObjectiveColor));
	Canvas->DrawItem(TitleItem);

	Position.Y += 20.0f;

	FString ObjectiveText = FString::Printf(TEXT("Complication: %s"), *CurrentMission.Complication);
	FCanvasTextItem ObjectiveItem(Position, FText::FromString(ObjectiveText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(ObjectiveItem);

	Position.Y += 20.0f;

	FString ExtractionText = FString::Printf(TEXT("Extract via: %s"), *CurrentMission.ExtractionCondition);
	FCanvasTextItem ExtractionItem(Position, FText::FromString(ExtractionText), GEngine->GetSmallFont(), FLinearColor(TextColor));
	Canvas->DrawItem(ExtractionItem);
}

void ANeonHUD::DrawExtractionIndicator()
{
	// Draw extraction status in top right
	FString ExtractionStatus = TEXT("Extraction: ACTIVE");
	FColor ExtractionColor = FColor::Green;

	FVector2D TextSize = FVector2D::ZeroVector;
	Canvas->TextSize(GEngine->GetSmallFont(), ExtractionStatus, TextSize.X, TextSize.Y);

	FVector2D Position = FVector2D(Canvas->SizeX - TextSize.X - 20.0f, 20.0f);
	FCanvasTextItem TextItem(Position, FText::FromString(ExtractionStatus), GEngine->GetSmallFont(), FLinearColor(ExtractionColor));
	Canvas->DrawItem(TextItem);
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
	FCanvasTileItem TileItem(Position, FVector2D(1, 1), FLinearColor(Color));
	TileItem.Size = Size;
	Canvas->DrawItem(TileItem);
}

void ANeonHUD::DrawBorderedRect(FVector2D Position, FVector2D Size, FColor BorderColor, FColor FillColor)
{
	// Draw fill
	DrawFilledRect(Position, Size, FillColor);

	// Draw border lines
	FLinearColor LineColor = FLinearColor(BorderColor);

	// Top
	FCanvasLineItem TopLine(Position, Position + FVector2D(Size.X, 0));
	TopLine.SetColor(LineColor);
	TopLine.LineThickness = 2.0f;
	Canvas->DrawItem(TopLine);

	// Bottom
	FCanvasLineItem BottomLine(Position + FVector2D(0, Size.Y), Position + Size);
	BottomLine.SetColor(LineColor);
	BottomLine.LineThickness = 2.0f;
	Canvas->DrawItem(BottomLine);

	// Left
	FCanvasLineItem LeftLine(Position, Position + FVector2D(0, Size.Y));
	LeftLine.SetColor(LineColor);
	LeftLine.LineThickness = 2.0f;
	Canvas->DrawItem(LeftLine);

	// Right
	FCanvasLineItem RightLine(Position + FVector2D(Size.X, 0), Position + Size);
	RightLine.SetColor(LineColor);
	RightLine.LineThickness = 2.0f;
	Canvas->DrawItem(RightLine);
}
