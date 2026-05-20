#pragma once

#include "CoreMinimal.h"
#include "MissionTypes.h"
#include "NeonAbility.generated.h"

UCLASS(Blueprintable, Abstract)
class NEONASCENDANT_API UNeonAbility : public UObject
{
	GENERATED_BODY()

public:
	UNeonAbility();

	// Ability identity
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	FString AbilityName;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	FString AbilityDescription;

	// Cooldown configuration
	UPROPERTY(BlueprintReadWrite, Category = "Abilities")
	float CooldownSeconds = 15.0f;

	// Cooldown state (read-only from Blueprint)
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	bool bIsOnCooldown = false;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	float CooldownRemaining = 0.0f;

	// Owner reference — must have UPROPERTY to prevent GC
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<AActor> OwnerActor;

	/** Attempt to activate this ability. Does nothing if on cooldown. */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void Activate();

	/** Override in subclasses to implement ability logic. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Abilities")
	void OnActivate();
	virtual void OnActivate_Implementation();

	/** Returns true when the ability is ready to use. */
	UFUNCTION(BlueprintPure, Category = "Abilities")
	bool CanActivate() const;

	/** Copy name, description, and cooldown from a lore data struct. */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void InitFromAbilityData(const FAscendantAbility& AbilityData);

	/** Set the actor that owns / instigates this ability. */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SetOwnerActor(AActor* NewOwner);

	virtual void BeginDestroy() override;

private:
	FTimerHandle CooldownTimerHandle;
	FTimerHandle CooldownTickTimerHandle;

	void StartCooldown();
	void OnCooldownFinished();
	void TickCooldown();
};
