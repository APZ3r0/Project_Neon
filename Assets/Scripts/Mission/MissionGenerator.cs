using System;
using System.Collections.Generic;
using UnityEngine;

namespace NeonAscendant.Mission
{
    /// <summary>
    /// Singleton mission generator with deterministic random generation
    /// </summary>
    public class MissionGenerator
    {
        private static MissionGenerator _instance;
        private System.Random _randomGenerator;
        private bool _hasSeed;

        public static MissionGenerator Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new MissionGenerator();
                }
                return _instance;
            }
        }

        private MissionGenerator()
        {
            _hasSeed = false;
        }

        /// <summary>
        /// Seed the random generator for deterministic mission generation
        /// </summary>
        public void SeedGenerator(int seed)
        {
            _randomGenerator = new System.Random(seed);
            _hasSeed = true;
        }

        /// <summary>
        /// Ensure random stream is initialized
        /// </summary>
        private void EnsureRandomStream()
        {
            if (!_hasSeed)
            {
                int seed = (int)(DateTime.UtcNow.Ticks & 0xFFFFFFFF);
                _randomGenerator = new System.Random(seed);
                _hasSeed = true;
            }
        }

        /// <summary>
        /// Generate a single mission brief
        /// </summary>
        public MissionBrief GenerateMissionBrief()
        {
            EnsureRandomStream();

            var districts = MissionData.GetDistricts();
            var factions = MissionData.GetFactions();
            var archetypes = MissionData.GetArchetypes();
            var weapons = MissionData.GetWeapons();
            var implants = MissionData.GetImplants();
            var complications = MissionData.GetComplications();
            var extractionConditions = MissionData.GetExtractionConditions();

            // Validate that all data arrays are non-empty
            Debug.Assert(districts.Count > 0, "Districts array is empty!");
            Debug.Assert(factions.Count > 0, "Factions array is empty!");
            Debug.Assert(archetypes.Count > 0, "Archetypes array is empty!");
            Debug.Assert(weapons.Count > 0, "Weapons array is empty!");
            Debug.Assert(implants.Count > 0, "Implants array is empty!");
            Debug.Assert(complications.Count > 0, "Complications array is empty!");
            Debug.Assert(extractionConditions.Count > 0, "ExtractionConditions array is empty!");

            var district = districts[_randomGenerator.Next(0, districts.Count)];
            var opposition = factions[_randomGenerator.Next(0, factions.Count)];
            var archetype = archetypes[_randomGenerator.Next(0, archetypes.Count)];
            var weapon = weapons[_randomGenerator.Next(0, weapons.Count)];
            var implant = implants[_randomGenerator.Next(0, implants.Count)];

            // Validate archetype has abilities before accessing
            Debug.Assert(archetype.SignatureAbilities.Count > 0, $"Archetype '{archetype.Name}' has no signature abilities!");
            var ability = archetype.SignatureAbilities[_randomGenerator.Next(0, archetype.SignatureAbilities.Count)];

            var complication = complications[_randomGenerator.Next(0, complications.Count)];
            var extractionCondition = extractionConditions[_randomGenerator.Next(0, extractionConditions.Count)];

            return new MissionBrief
            {
                District = district,
                Opposition = opposition,
                Archetype = archetype,
                PrimaryWeapon = weapon,
                BackupImplant = implant,
                FeaturedAbility = ability,
                Complication = complication,
                ExtractionCondition = extractionCondition
            };
        }

        /// <summary>
        /// Generate multiple mission briefs
        /// </summary>
        public List<MissionBrief> GenerateMissionBriefs(int count)
        {
            Debug.Assert(count > 0, "GenerateMissionBriefs requires count to be positive.");

            var briefs = new List<MissionBrief>(count);
            for (int i = 0; i < count; i++)
            {
                briefs.Add(GenerateMissionBrief());
            }

            return briefs;
        }
    }
}
