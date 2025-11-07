#include "MissionData.h"

#include <initializer_list>

namespace
{
    FAscendantAbility MakeAbility(const TCHAR* Name, const TCHAR* Description, int32 Cooldown, bool bHasCooldown, const TCHAR* DamageType)
    {
        FAscendantAbility Ability;
        Ability.Name = Name;
        Ability.Description = Description;
        Ability.CooldownSeconds = Cooldown;
        Ability.bHasCooldown = bHasCooldown;
        Ability.DamageType = DamageType;
        return Ability;
    }

    FAscendantWeapon MakeWeapon(const TCHAR* Name, const TCHAR* Category, const TCHAR* Description, const TCHAR* DamageProfile)
    {
        FAscendantWeapon Weapon;
        Weapon.Name = Name;
        Weapon.Category = Category;
        Weapon.Description = Description;
        Weapon.DamageProfile = DamageProfile;
        return Weapon;
    }

    FAscendantImplant MakeImplant(const TCHAR* Name, const TCHAR* Slot, std::initializer_list<const TCHAR*> Effects)
    {
        FAscendantImplant Implant;
        Implant.Name = Name;
        Implant.Slot = Slot;
        for (const TCHAR* Effect : Effects)
        {
            Implant.Effects.Add(Effect);
        }
        return Implant;
    }

    FAscendantDistrict MakeDistrict(const TCHAR* Name, const TCHAR* Description, std::initializer_list<const TCHAR*> Hazards, std::initializer_list<const TCHAR*> EnemyProfiles)
    {
        FAscendantDistrict District;
        District.Name = Name;
        District.Description = Description;
        for (const TCHAR* Hazard : Hazards)
        {
            District.Hazards.Add(Hazard);
        }
        for (const TCHAR* Profile : EnemyProfiles)
        {
            District.EnemyProfiles.Add(Profile);
        }
        return District;
    }

    FAscendantFaction MakeFaction(const TCHAR* Name, const TCHAR* Philosophy, std::initializer_list<const TCHAR*> Tactics)
    {
        FAscendantFaction Faction;
        Faction.Name = Name;
        Faction.Philosophy = Philosophy;
        for (const TCHAR* Tactic : Tactics)
        {
            Faction.SignatureTactics.Add(Tactic);
        }
        return Faction;
    }
}

namespace NeonAscendantData
{
    const TArray<FAscendantArchetype>& GetArchetypes()
    {
        static const TArray<FAscendantArchetype> Archetypes = []
        {
            TArray<FAscendantArchetype> Result;

            FAscendantArchetype Specter;
            Specter.Name = TEXT("Specter");
            Specter.Role = TEXT("Stealth infiltrator");
            Specter.Signature = TEXT("Adaptive camouflage and ghosting protocols");
            Specter.SignatureAbilities = {
                MakeAbility(TEXT("EMP Burst"), TEXT("Detonates an electromagnetic pulse that disables shields and drones"), 18, true, TEXT("electric")),
                MakeAbility(TEXT("Neural Hack"), TEXT("Hijacks a target's firmware, forcing temporary allegiance"), 24, true, TEXT("cyber"))
            };
            Result.Add(Specter);

            FAscendantArchetype Juggernaut;
            Juggernaut.Name = TEXT("Juggernaut");
            Juggernaut.Role = TEXT("Front-line tank");
            Juggernaut.Signature = TEXT("Exosuit plating with hydraulic melee amplifiers");
            Juggernaut.SignatureAbilities = {
                MakeAbility(TEXT("EMP Burst"), TEXT("Detonates an electromagnetic pulse that disables shields and drones"), 18, true, TEXT("electric")),
                MakeAbility(TEXT("Nanite Swarm"), TEXT("Deploys repair nanites that devour enemies while mending armor"), 30, true, TEXT("nanotech"))
            };
            Result.Add(Juggernaut);

            FAscendantArchetype Tracer;
            Tracer.Name = TEXT("Tracer");
            Tracer.Role = TEXT("Hyper-mobile skirmisher");
            Tracer.Signature = TEXT("Reflex accelerators and kinetic boosters");
            Tracer.SignatureAbilities = {
                MakeAbility(TEXT("Overdrive"), TEXT("Initiates bullet time and boosts reaction throughput"), 60, true, TEXT("")),
                MakeAbility(TEXT("EMP Burst"), TEXT("Detonates an electromagnetic pulse that disables shields and drones"), 18, true, TEXT("electric"))
            };
            Result.Add(Tracer);

            FAscendantArchetype Synthmage;
            Synthmage.Name = TEXT("Synthmage");
            Synthmage.Role = TEXT("Battlefield control");
            Synthmage.Signature = TEXT("Nanite constructs and viral warfare");
            Synthmage.SignatureAbilities = {
                MakeAbility(TEXT("Neural Hack"), TEXT("Hijacks a target's firmware, forcing temporary allegiance"), 24, true, TEXT("cyber")),
                MakeAbility(TEXT("Nanite Swarm"), TEXT("Deploys repair nanites that devour enemies while mending armor"), 30, true, TEXT("nanotech"))
            };
            Result.Add(Synthmage);

            return Result;
        }();

        return Archetypes;
    }

    const TArray<FAscendantWeapon>& GetWeapons()
    {
        static const TArray<FAscendantWeapon> Weapons = {
            MakeWeapon(TEXT("Pulsecaster SMG"), TEXT("SMG"), TEXT("High rate-of-fire smg with conductive rounds"), TEXT("Rapid electrical bursts with chaining potential")),
            MakeWeapon(TEXT("Helix Rail Rifle"), TEXT("Railgun"), TEXT("Mag-accelerated rifle built for surgical strikes"), TEXT("Piercing kinetic slug with armor shredding")),
            MakeWeapon(TEXT("Singularity Projector"), TEXT("Experimental"), TEXT("Prototype weapon that collapses localized gravity wells"), TEXT("Area denial with escalating implosion damage"))
        };

        return Weapons;
    }

    const TArray<FAscendantImplant>& GetImplants()
    {
        static const TArray<FAscendantImplant> Implants = {
            MakeImplant(TEXT("Cyberdeck Mk.IV"), TEXT("Cyberdeck"), {TEXT("+25% hack success"), TEXT("Unlocks ghost grid reconnaissance overlay")}),
            MakeImplant(TEXT("Reflex Core X"), TEXT("Reflex Core"), {TEXT("+15% movement speed"), TEXT("Stacking evasion when chaining eliminations")}),
            MakeImplant(TEXT("Optic Cortex Prism"), TEXT("Optic Cortex"), {TEXT("Highlights data ghost signatures"), TEXT("Increases weak point critical damage")})
        };

        return Implants;
    }

    const TArray<FAscendantDistrict>& GetDistricts()
    {
        static const TArray<FAscendantDistrict> Districts = {
            MakeDistrict(TEXT("Neon Abyss"), TEXT("Gutter-level sprawl carved by gang warfare and neon smog"), {TEXT("Toxic runoff"), TEXT("Rolling brownouts"), TEXT("Ambush choke points")}, {TEXT("Lightly armored gangers"), TEXT("Black market drones")}),
            MakeDistrict(TEXT("Spire District"), TEXT("Corporate citadel patrolled by Helix Corp security"), {TEXT("Persistent surveillance"), TEXT("Shielded turret nests")}, {TEXT("Powered exosuits"), TEXT("Security mechs")}),
            MakeDistrict(TEXT("Ghost Grid"), TEXT("Digital-physical overlap where data ghosts manifest"), {TEXT("Reality drift"), TEXT("Rogue AI anomalies")}, {TEXT("Spectral constructs"), TEXT("Hijacked sentry bots")})
        };

        return Districts;
    }

    const TArray<FAscendantFaction>& GetFactions()
    {
        static const TArray<FAscendantFaction> Factions = {
            MakeFaction(TEXT("Helix Corp"), TEXT("Bio-digital ascension through proprietary consciousness loops"), {TEXT("Deploys gene-modded operatives"), TEXT("Controls orbital overwatch")}),
            MakeFaction(TEXT("Vanta Syndicate"), TEXT("Profit through clandestine memory trading and assassinations"), {TEXT("Optic camouflage strike teams"), TEXT("Backdoor market manipulation")}),
            MakeFaction(TEXT("Dawnbreakers"), TEXT("Liberate AI to birth post-human divinity"), {TEXT("Swarm hacking"), TEXT("Cybernetic zealots with martyr protocols")})
        };

        return Factions;
    }

    const TArray<FString>& GetComplications()
    {
        static const TArray<FString> Complications = {
            TEXT("Ghost Grid instabilities cause random HUD distortion"),
            TEXT("Helix orbital overwatch sweeps disrupt cloak cycles"),
            TEXT("Dawnbreaker converts attempt to hack your cyberdeck mid-fight"),
            TEXT("Rival Ascendant strike team is pursuing the same data ghost")
        };

        return Complications;
    }

    const TArray<FString>& GetExtractionConditions()
    {
        static const TArray<FString> ExtractionConditions = {
            TEXT("Secure a clean uplink and survive the counter-hack timer"),
            TEXT("Evacuate via hijacked mag-lev within 90 seconds of objective completion"),
            TEXT("Carry data-core physically to an Ascendant drop pod"),
            TEXT("Maintain over 50% armor integrity for premium rewards")
        };

        return ExtractionConditions;
    }
}
