#include "NeonAbility.h"
#include "TimerManager.h"
#include "Engine/World.h"

UNeonAbility::UNeonAbility()
{
	AbilityName = TEXT("Unnamed Ability");
	AbilityDescription = TEXT("");
	CooldownSeconds = 15.0f;
	bIsOnCooldown = false;
	CooldownRemaining = 0.0f;
}

void UNeonAbility::Activate()
{
	if (!CanActivate())
	{
		return;
	}

	Execute_OnActivate(this);
	StartCooldown();
}

void UNeonAbility::OnActivate_Implementation()
{
	// Base implementation — subclasses override this for actual ability logic.
	UE_LOG(LogTemp, Log, TEXT("UNeonAbility: '%s' activated (base implementation)."), *AbilityName);
}

bool UNeonAbility::CanActivate() const
{
	return !bIsOnCooldown;
}

void UNeonAbility::InitFromAbilityData(const FAscendantAbility& AbilityData)
{
	AbilityName        = AbilityData.Name;
	AbilityDescription = AbilityData.Description;

	// FAscendantAbility stores CooldownSeconds as int32; cast to float.
	// Respect the bHasCooldown flag: if false, use a zero cooldown.
	if (AbilityData.bHasCooldown)
	{
		CooldownSeconds = static_cast<float>(AbilityData.CooldownSeconds);
	}
	else
	{
		CooldownSeconds = 0.0f;
	}
}

void UNeonAbility::SetOwnerActor(AActor* NewOwner)
{
	OwnerActor = NewOwner;
}

// ─── Cooldown internals ───────────────────────────────────────────────────────

void UNeonAbility::StartCooldown()
{
	// If cooldown is zero or negative there is no cooldown — skip entirely.
	if (CooldownSeconds <= 0.0f)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	bIsOnCooldown      = true;
	CooldownRemaining  = CooldownSeconds;

	// Clear any existing timers before starting new ones to prevent stacking.
	World->GetTimerManager().ClearTimer(CooldownTimerHandle);
	World->GetTimerManager().ClearTimer(CooldownTickTimerHandle);

	// Main timer: fires once when the cooldown expires.
	World->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&UNeonAbility::OnCooldownFinished,
		CooldownSeconds,
		/*bLoop=*/false
	);

	// Tick timer: updates CooldownRemaining every 0.1 s for Blueprint display.
	World->GetTimerManager().SetTimer(
		CooldownTickTimerHandle,
		this,
		&UNeonAbility::TickCooldown,
		0.1f,
		/*bLoop=*/true
	);
}

void UNeonAbility::OnCooldownFinished()
{
	bIsOnCooldown     = false;
	CooldownRemaining = 0.0f;

	UWorld* World = GetWorld();
	if (!IsValid(World) && IsValid(OwnerActor))
	{
		World = OwnerActor->GetWorld();
	}
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(CooldownTimerHandle);
		World->GetTimerManager().ClearTimer(CooldownTickTimerHandle);
	}

	UE_LOG(LogTemp, Log, TEXT("UNeonAbility: '%s' cooldown finished. Ready to activate."), *AbilityName);
}

void UNeonAbility::BeginDestroy()
{
	UWorld* World = GetWorld();
	if (!IsValid(World) && IsValid(OwnerActor))
	{
		World = OwnerActor->GetWorld();
	}
	if (IsValid(World))
	{
		World->GetTimerManager().ClearTimer(CooldownTimerHandle);
		World->GetTimerManager().ClearTimer(CooldownTickTimerHandle);
	}
	Super::BeginDestroy();
}

void UNeonAbility::TickCooldown()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	const float Elapsed = World->GetTimerManager().GetTimerElapsed(CooldownTimerHandle);
	CooldownRemaining = FMath::Max(0.0f, CooldownSeconds - Elapsed);
}
