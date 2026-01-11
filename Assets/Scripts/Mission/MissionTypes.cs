using System;
using System.Collections.Generic;
using UnityEngine;

namespace NeonAscendant.Mission
{
    /// <summary>
    /// Character ability definition
    /// </summary>
    [Serializable]
    public struct AscendantAbility
    {
        public string Name;
        public string Description;
        public int CooldownSeconds;
        public bool HasCooldown;
        public string DamageType;
    }

    /// <summary>
    /// Player character archetype (class)
    /// </summary>
    [Serializable]
    public struct AscendantArchetype
    {
        public string Name;
        public string Role;
        public string Signature;
        public List<AscendantAbility> SignatureAbilities;
    }

    /// <summary>
    /// Weapon definition
    /// </summary>
    [Serializable]
    public struct AscendantWeapon
    {
        public string Name;
        public string Category;
        public string Description;
        public string DamageProfile;
    }

    /// <summary>
    /// Character augmentation implant
    /// </summary>
    [Serializable]
    public struct AscendantImplant
    {
        public string Name;
        public string Slot;
        public List<string> Effects;
    }

    /// <summary>
    /// Mission location district
    /// </summary>
    [Serializable]
    public struct AscendantDistrict
    {
        public string Name;
        public string Description;
        public List<string> Hazards;
        public List<string> EnemyProfiles;
    }

    /// <summary>
    /// Enemy faction
    /// </summary>
    [Serializable]
    public struct AscendantFaction
    {
        public string Name;
        public string Philosophy;
        public List<string> SignatureTactics;
    }

    /// <summary>
    /// Complete mission briefing with all parameters
    /// </summary>
    [Serializable]
    public struct MissionBrief
    {
        public AscendantDistrict District;
        public AscendantFaction Opposition;
        public AscendantArchetype Archetype;
        public AscendantWeapon PrimaryWeapon;
        public AscendantImplant BackupImplant;
        public AscendantAbility FeaturedAbility;
        public string Complication;
        public string ExtractionCondition;
    }
}
