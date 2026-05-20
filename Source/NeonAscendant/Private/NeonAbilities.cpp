#include "NeonAbilities.h"
#include "NeonEnemy.h"
#include "NeonCharacter.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "TimerManager.h"

// ─── UEMPBurstAbility ─────────────────────────────────────────────────────────

void UEMPBurstAbility::OnActivate_Implementation()
{
	UWorld* World = GetWorld();
	if (!IsValid(World) || !IsValid(OwnerActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("UEMPBurstAbility: Cannot activate — World or OwnerActor is invalid."));
		return;
	}

	const FVector Origin = OwnerActor->GetActorLocation();
	int32 HitCount = 0;

	// Iterate all ANeonEnemy actors in the world and apply damage to those in range.
	for (TActorIterator<ANeonEnemy> It(World); It; ++It)
	{
		ANeonEnemy* Enemy = *It;
		if (!IsValid(Enemy) || Enemy->bIsDead)
		{
			continue;
		}

		const float DistanceSq = FVector::DistSquared(Origin, Enemy->GetActorLocation());
		if (DistanceSq <= FMath::Square(BlastRadius))
		{
			FDamageEvent DamageEvent;
			Enemy->TakeDamage(EmpDamage, DamageEvent, nullptr, OwnerActor.Get());
			++HitCount;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("UEMPBurstAbility '%s': EMP detonated at (%.0f, %.0f, %.0f) — %d enemies hit."),
		*AbilityName,
		Origin.X, Origin.Y, Origin.Z,
		HitCount);
}

// ─── UCloakFieldAbility ───────────────────────────────────────────────────────

void UCloakFieldAbility::OnActivate_Implementation()
{
	UWorld* World = GetWorld();
	if (!IsValid(World) || !IsValid(OwnerActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCloakFieldAbility: Cannot activate — World or OwnerActor is invalid."));
		return;
	}

	ANeonCharacter* Character = Cast<ANeonCharacter>(OwnerActor);
	if (!IsValid(Character))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCloakFieldAbility: OwnerActor is not an ANeonCharacter — cloak aborted."));
		return;
	}

	// Hide the character mesh.
	USkeletalMeshComponent* Mesh = Character->GetMesh();
	if (IsValid(Mesh))
	{
		Mesh->SetVisibility(false);
	}

	const float CloakDuration = CooldownSeconds * 0.4f;

	UE_LOG(LogTemp, Log, TEXT("UCloakFieldAbility '%s': Cloak activated for %.1f seconds."),
		*AbilityName, CloakDuration);

	// Restore visibility after cloakDuration using a weak lambda so we don't extend
	// the lifetime of the ability object if it gets collected.
	World->GetTimerManager().SetTimer(
		CloakRestoreTimerHandle,
		FTimerDelegate::CreateWeakLambda(Character, [Mesh]()
		{
			if (IsValid(Mesh))
			{
				Mesh->SetVisibility(true);
				UE_LOG(LogTemp, Log, TEXT("UCloakFieldAbility: Cloak expired — mesh visibility restored."));
			}
		}),
		FMath::Max(0.1f, CloakDuration),
		/*bLoop=*/false
	);
}
