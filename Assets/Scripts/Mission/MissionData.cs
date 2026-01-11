using System.Collections.Generic;
using UnityEngine;

namespace NeonAscendant.Mission
{
    /// <summary>
    /// Static lore data for mission generation
    /// All data is hard-coded for deterministic generation
    /// </summary>
    public static class MissionData
    {
        private static List<AscendantArchetype> _archetypes;
        private static List<AscendantWeapon> _weapons;
        private static List<AscendantImplant> _implants;
        private static List<AscendantDistrict> _districts;
        private static List<AscendantFaction> _factions;
        private static List<string> _complications;
        private static List<string> _extractionConditions;

        public static List<AscendantArchetype> GetArchetypes()
        {
            if (_archetypes == null)
            {
                _archetypes = new List<AscendantArchetype>
                {
                    new AscendantArchetype
                    {
                        Name = "Specter",
                        Role = "Stealth infiltrator",
                        Signature = "Adaptive camouflage and ghosting protocols",
                        SignatureAbilities = new List<AscendantAbility>
                        {
                            new AscendantAbility { Name = "EMP Burst", Description = "Detonates an electromagnetic pulse that disables shields and drones", CooldownSeconds = 18, HasCooldown = true, DamageType = "electric" },
                            new AscendantAbility { Name = "Neural Hack", Description = "Hijacks a target's firmware, forcing temporary allegiance", CooldownSeconds = 24, HasCooldown = true, DamageType = "cyber" }
                        }
                    },
                    new AscendantArchetype
                    {
                        Name = "Juggernaut",
                        Role = "Front-line tank",
                        Signature = "Exosuit plating with hydraulic melee amplifiers",
                        SignatureAbilities = new List<AscendantAbility>
                        {
                            new AscendantAbility { Name = "EMP Burst", Description = "Detonates an electromagnetic pulse that disables shields and drones", CooldownSeconds = 18, HasCooldown = true, DamageType = "electric" },
                            new AscendantAbility { Name = "Nanite Swarm", Description = "Deploys repair nanites that devour enemies while mending armor", CooldownSeconds = 30, HasCooldown = true, DamageType = "nanotech" }
                        }
                    },
                    new AscendantArchetype
                    {
                        Name = "Tracer",
                        Role = "Hyper-mobile skirmisher",
                        Signature = "Reflex accelerators and kinetic boosters",
                        SignatureAbilities = new List<AscendantAbility>
                        {
                            new AscendantAbility { Name = "Overdrive", Description = "Initiates bullet time and boosts reaction throughput", CooldownSeconds = 60, HasCooldown = true, DamageType = "" },
                            new AscendantAbility { Name = "EMP Burst", Description = "Detonates an electromagnetic pulse that disables shields and drones", CooldownSeconds = 18, HasCooldown = true, DamageType = "electric" }
                        }
                    },
                    new AscendantArchetype
                    {
                        Name = "Synthmage",
                        Role = "Battlefield control",
                        Signature = "Nanite constructs and viral warfare",
                        SignatureAbilities = new List<AscendantAbility>
                        {
                            new AscendantAbility { Name = "Neural Hack", Description = "Hijacks a target's firmware, forcing temporary allegiance", CooldownSeconds = 24, HasCooldown = true, DamageType = "cyber" },
                            new AscendantAbility { Name = "Nanite Swarm", Description = "Deploys repair nanites that devour enemies while mending armor", CooldownSeconds = 30, HasCooldown = true, DamageType = "nanotech" }
                        }
                    }
                };
            }
            return _archetypes;
        }

        public static List<AscendantWeapon> GetWeapons()
        {
            if (_weapons == null)
            {
                _weapons = new List<AscendantWeapon>
                {
                    new AscendantWeapon { Name = "Pulsecaster SMG", Category = "SMG", Description = "High rate-of-fire smg with conductive rounds", DamageProfile = "Rapid electrical bursts with chaining potential" },
                    new AscendantWeapon { Name = "Helix Rail Rifle", Category = "Railgun", Description = "Mag-accelerated rifle built for surgical strikes", DamageProfile = "Piercing kinetic slug with armor shredding" },
                    new AscendantWeapon { Name = "Singularity Projector", Category = "Experimental", Description = "Prototype weapon that collapses localized gravity wells", DamageProfile = "Area denial with escalating implosion damage" }
                };
            }
            return _weapons;
        }

        public static List<AscendantImplant> GetImplants()
        {
            if (_implants == null)
            {
                _implants = new List<AscendantImplant>
                {
                    new AscendantImplant { Name = "Cyberdeck Mk.IV", Slot = "Cyberdeck", Effects = new List<string> { "+25% hack success", "Unlocks ghost grid reconnaissance overlay" } },
                    new AscendantImplant { Name = "Reflex Core X", Slot = "Reflex Core", Effects = new List<string> { "+15% movement speed", "Stacking evasion when chaining eliminations" } },
                    new AscendantImplant { Name = "Optic Cortex Prism", Slot = "Optic Cortex", Effects = new List<string> { "Highlights data ghost signatures", "Increases weak point critical damage" } }
                };
            }
            return _implants;
        }

        public static List<AscendantDistrict> GetDistricts()
        {
            if (_districts == null)
            {
                _districts = new List<AscendantDistrict>
                {
                    new AscendantDistrict { Name = "Neon Abyss", Description = "Gutter-level sprawl carved by gang warfare and neon smog", Hazards = new List<string> { "Toxic runoff", "Rolling brownouts", "Ambush choke points" }, EnemyProfiles = new List<string> { "Lightly armored gangers", "Black market drones" } },
                    new AscendantDistrict { Name = "Spire District", Description = "Corporate citadel patrolled by Helix Corp security", Hazards = new List<string> { "Persistent surveillance", "Shielded turret nests" }, EnemyProfiles = new List<string> { "Powered exosuits", "Security mechs" } },
                    new AscendantDistrict { Name = "Ghost Grid", Description = "Digital-physical overlap where data ghosts manifest", Hazards = new List<string> { "Reality drift", "Rogue AI anomalies" }, EnemyProfiles = new List<string> { "Spectral constructs", "Hijacked sentry bots" } }
                };
            }
            return _districts;
        }

        public static List<AscendantFaction> GetFactions()
        {
            if (_factions == null)
            {
                _factions = new List<AscendantFaction>
                {
                    new AscendantFaction { Name = "Helix Corp", Philosophy = "Bio-digital ascension through proprietary consciousness loops", SignatureTactics = new List<string> { "Deploys gene-modded operatives", "Controls orbital overwatch" } },
                    new AscendantFaction { Name = "Vanta Syndicate", Philosophy = "Profit through clandestine memory trading and assassinations", SignatureTactics = new List<string> { "Optic camouflage strike teams", "Backdoor market manipulation" } },
                    new AscendantFaction { Name = "Dawnbreakers", Philosophy = "Liberate AI to birth post-human divinity", SignatureTactics = new List<string> { "Swarm hacking", "Cybernetic zealots with martyr protocols" } }
                };
            }
            return _factions;
        }

        public static List<string> GetComplications()
        {
            if (_complications == null)
            {
                _complications = new List<string>
                {
                    "Ghost Grid instabilities cause random HUD distortion",
                    "Helix orbital overwatch sweeps disrupt cloak cycles",
                    "Dawnbreaker converts attempt to hack your cyberdeck mid-fight",
                    "Rival Ascendant strike team is pursuing the same data ghost"
                };
            }
            return _complications;
        }

        public static List<string> GetExtractionConditions()
        {
            if (_extractionConditions == null)
            {
                _extractionConditions = new List<string>
                {
                    "Secure a clean uplink and survive the counter-hack timer",
                    "Evacuate via hijacked mag-lev within 90 seconds of objective completion",
                    "Carry data-core physically to an Ascendant drop pod",
                    "Maintain over 50% armor integrity for premium rewards"
                };
            }
            return _extractionConditions;
        }
    }
}
