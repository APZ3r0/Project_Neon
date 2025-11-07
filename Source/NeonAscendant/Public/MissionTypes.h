#pragma once

#include "CoreMinimal.h"
#include "MissionTypes.generated.h"

USTRUCT(BlueprintType)
struct FAscendantAbility
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
    int32 CooldownSeconds = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
    bool bHasCooldown = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Ability")
    FString DamageType;
};

USTRUCT(BlueprintType)
struct FAscendantArchetype
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archetype")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archetype")
    FString Role;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archetype")
    FString Signature;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Archetype")
    TArray<FAscendantAbility> SignatureAbilities;
};

USTRUCT(BlueprintType)
struct FAscendantWeapon
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
    FString Category;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon")
    FString DamageProfile;
};

USTRUCT(BlueprintType)
struct FAscendantImplant
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Implant")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Implant")
    FString Slot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Implant")
    TArray<FString> Effects;
};

USTRUCT(BlueprintType)
struct FAscendantDistrict
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="District")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="District")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="District")
    TArray<FString> Hazards;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="District")
    TArray<FString> EnemyProfiles;
};

USTRUCT(BlueprintType)
struct FAscendantFaction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Faction")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Faction")
    FString Philosophy;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Faction")
    TArray<FString> SignatureTactics;
};

USTRUCT(BlueprintType)
struct FMissionBrief
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
    FAscendantDistrict District;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
    FAscendantFaction Opposition;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
    FAscendantArchetype Archetype;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
    FAscendantWeapon PrimaryWeapon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
    FAscendantImplant BackupImplant;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
    FAscendantAbility FeaturedAbility;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
    FString Complication;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mission")
    FString ExtractionCondition;
};
