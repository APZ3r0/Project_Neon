using System.Collections.Generic;
using UnityEngine;
using NeonAscendant.Mission;

namespace NeonAscendant.Core
{
    /// <summary>
    /// Game manager that integrates mission generator and handles game initialization
    /// </summary>
    public class NeonGameManager : MonoBehaviour
    {
        [Header("Prefabs")]
        [SerializeField] private GameObject enemyPrefab;
        [SerializeField] private GameObject hazardPrefab;

        [Header("Spawn Configuration")]
        [SerializeField] private Transform player;

        // Configuration constants
        private const float EnemySpawnMinDistance = -20.0f;
        private const float EnemySpawnMaxDistance = 20.0f;
        private const float EnemySpawnHeightOffset = 1.0f;
        private const int DefaultEnemyCount = 3;

        private const float HazardSpawnMinDistance = -30.0f;
        private const float HazardSpawnMaxDistance = 30.0f;
        private const float HazardSpawnHeight = 1.0f;
        private const int MinHazardCount = 2;
        private const int MaxHazardCount = 3;

        // State
        private MissionBrief currentMission;
        private List<GameObject> spawnedEnemies = new List<GameObject>();
        private List<GameObject> activeHazards = new List<GameObject>();
        private System.Random spawnRandom;

        // Singleton instance
        private static NeonGameManager _instance;
        public static NeonGameManager Instance => _instance;

        public MissionBrief CurrentMission => currentMission;

        private void Awake()
        {
            // Singleton pattern
            if (_instance != null && _instance != this)
            {
                Destroy(gameObject);
                return;
            }

            _instance = this;
            DontDestroyOnLoad(gameObject);

            // Initialize random generator
            spawnRandom = new System.Random();
        }

        private void Start()
        {
            // Auto-start a mission on level load
            StartNewMission();
        }

        /// <summary>
        /// Generate and start a new mission
        /// </summary>
        public void StartNewMission()
        {
            // Generate mission
            currentMission = MissionGenerator.Instance.GenerateMissionBrief();

            Debug.Log($"NEW MISSION GENERATED:");
            Debug.Log($"District: {currentMission.District.Name}");
            Debug.Log($"Opposition: {currentMission.Opposition.Name}");
            Debug.Log($"Archetype: {currentMission.Archetype.Name}");
            Debug.Log($"Weapon: {currentMission.PrimaryWeapon.Name}");
            Debug.Log($"Implant: {currentMission.BackupImplant.Name}");
            Debug.Log($"Complication: {currentMission.Complication}");
            Debug.Log($"Extraction: {currentMission.ExtractionCondition}");

            // Spawn enemies and hazards
            SpawnEnemiesForMission(currentMission, DefaultEnemyCount);
            SpawnHazardsForMission(currentMission);
        }

        /// <summary>
        /// Spawn enemies based on mission data
        /// </summary>
        public void SpawnEnemiesForMission(MissionBrief mission, int enemyCount = 3)
        {
            if (enemyPrefab == null)
            {
                Debug.LogWarning("Enemy prefab not assigned!");
                return;
            }

            // Clear existing enemies
            foreach (var enemy in spawnedEnemies)
            {
                if (enemy != null)
                    Destroy(enemy);
            }
            spawnedEnemies.Clear();

            // Determine spawn origin
            Vector3 spawnOrigin = player != null ? player.position : Vector3.zero;

            Debug.Log($"Spawning {enemyCount} enemies for opposition: {mission.Opposition.Name}");

            for (int i = 0; i < enemyCount; i++)
            {
                // Random spawn location around player
                Vector3 spawnLocation = spawnOrigin + new Vector3(
                    (float)(spawnRandom.NextDouble() * (EnemySpawnMaxDistance - EnemySpawnMinDistance) + EnemySpawnMinDistance),
                    EnemySpawnHeightOffset,
                    (float)(spawnRandom.NextDouble() * (EnemySpawnMaxDistance - EnemySpawnMinDistance) + EnemySpawnMinDistance)
                );

                // Spawn enemy
                GameObject enemy = Instantiate(enemyPrefab, spawnLocation, Quaternion.identity);
                if (enemy != null)
                {
                    spawnedEnemies.Add(enemy);
#if !UNITY_EDITOR && !DEVELOPMENT_BUILD
                    // Only log in development builds
#else
                    Debug.Log($"Spawned enemy {i + 1} at location {spawnLocation}");
#endif
                }
            }
        }

        /// <summary>
        /// Spawn hazards based on mission data
        /// </summary>
        public void SpawnHazardsForMission(MissionBrief mission)
        {
            if (hazardPrefab == null)
            {
                Debug.LogWarning("Hazard prefab not assigned!");
                return;
            }

            // Clear existing hazards
            foreach (var hazard in activeHazards)
            {
                if (hazard != null)
                    Destroy(hazard);
            }
            activeHazards.Clear();

            // Spawn hazards based on mission complication
            int hazardCount = spawnRandom.Next(MinHazardCount, MaxHazardCount + 1);

            Debug.Log($"Spawning {hazardCount} hazards for district {mission.District.Name}");

            for (int i = 0; i < hazardCount; i++)
            {
                // Calculate spawn position: random location around the level
                Vector3 spawnLocation = new Vector3(
                    (float)(spawnRandom.NextDouble() * (HazardSpawnMaxDistance - HazardSpawnMinDistance) + HazardSpawnMinDistance),
                    HazardSpawnHeight,
                    (float)(spawnRandom.NextDouble() * (HazardSpawnMaxDistance - HazardSpawnMinDistance) + HazardSpawnMinDistance)
                );

                // Spawn the hazard
                GameObject newHazard = Instantiate(hazardPrefab, spawnLocation, Quaternion.identity);
                if (newHazard != null)
                {
                    // Randomize hazard type and properties
                    var hazardComponent = newHazard.GetComponent<DistrictHazard>();
                    if (hazardComponent != null)
                    {
                        // Set random hazard type (5 types total: 0-4)
                        int hazardTypeIndex = spawnRandom.Next(0, 5);
                        hazardComponent.SetHazardType((HazardType)hazardTypeIndex);
                        hazardComponent.SetDamagePerSecond((float)(spawnRandom.NextDouble() * 10.0f + 5.0f));
                        hazardComponent.SetEffectRadius((float)(spawnRandom.NextDouble() * 300.0f + 300.0f));
                    }

                    activeHazards.Add(newHazard);

#if !UNITY_EDITOR && !DEVELOPMENT_BUILD
                    // Only log in development builds
#else
                    Debug.Log($"Spawned hazard {i + 1} at location {spawnLocation}");
#endif
                }
            }
        }
    }
}
