#pragma once

#include "CoreMinimal.h"
#include "NeonAbility.h"
#include "NeonAbilities.generated.h"

/**
 * UEMPBurstAbility
 *
 * Detonates an electromagnetic pulse that damages all ANeonEnemy actors within
 * 800 units of the owning actor.  Matches the "EMP Burst" lore entry.
 */
UCLASS(Blueprintable)
class NEONASCENDANT_API UEMPBurstAbility : public UNeonAbility
{
	GENERATED_BODY()

public:
	/** Radius (Unreal units) within which enemies receive the EMP damage hit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|EMP")
	float BlastRadius = 800.0f;

	/** Flat damage applied to each enemy inside the blast radius. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities|EMP")
	float EmpDamage = 25.0f;

	virtual void OnActivate_Implementation() override;
};

/**
 * UCloakFieldAbility
 *
 * Renders the owning ANeonCharacter's mesh invisible for a short window
 * (CooldownSeconds * 0.4 seconds) then restores visibility.
 * Matches the "Cloak Field" lore entry.
 */
UCLASS(Blueprintable)
class NEONASCENDANT_API UCloakFieldAbility : public UNeonAbility
{
	GENERATED_BODY()

public:
	virtual void OnActivate_Implementation() override;

private:
	FTimerHandle CloakRestoreTimerHandle;
};
