using System.Collections.Generic;
using UnityEngine;

namespace NeonAscendant.Core
{
    /// <summary>
    /// Hazard type based on district data
    /// </summary>
    public enum HazardType
    {
        Thermal = 0,
        Electrical = 1,
        Toxic = 2,
        Radiation = 3,
        Cryogenic = 4
    }

    /// <summary>
    /// District environmental hazard that damages players and enemies
    /// </summary>
    [RequireComponent(typeof(SphereCollider))]
    public class DistrictHazard : MonoBehaviour
    {
        [Header("Hazard Configuration")]
        [SerializeField] private HazardType hazardType = HazardType.Thermal;
        [SerializeField] private float damagePerSecond = 10.0f;
        [SerializeField] private float effectRadius = 5.0f;
        [SerializeField] private bool isActive = true;

        [Header("Visual")]
        [SerializeField] private Color hazardColor = Color.red;
        [SerializeField] private float visualIntensity = 1.0f;
        [SerializeField] private ParticleSystem hazardEffect;

        [Header("Damage Settings")]
        [SerializeField] private float damageTickRate = 1.0f; // Damage every X seconds

        private SphereCollider hazardVolume;
        private Dictionary<GameObject, float> lastDamageTime = new Dictionary<GameObject, float>();
        private List<GameObject> overlappingActors = new List<GameObject>();

        private const float TickInterval = 0.1f;
        private float tickTimer = 0f;

        private void Awake()
        {
            hazardVolume = GetComponent<SphereCollider>();
            hazardVolume.isTrigger = true;
            hazardVolume.radius = effectRadius;
        }

        private void Start()
        {
            CreateHazardEffects();
        }

        private void Update()
        {
            if (!isActive)
                return;

            tickTimer += Time.deltaTime;
            if (tickTimer >= TickInterval)
            {
                tickTimer = 0f;
                ApplyDamageToOverlapping();
            }
        }

        /// <summary>
        /// Apply damage to all overlapping actors
        /// </summary>
        private void ApplyDamageToOverlapping()
        {
            // Clean up null references
            overlappingActors.RemoveAll(actor => actor == null);

            foreach (var actor in overlappingActors)
            {
                if (actor == null)
                    continue;

                // Check if actor is a character or enemy - damage both
                var damageable = actor.GetComponent<Gameplay.IDamageable>();
                if (damageable != null)
                {
                    ApplyHazardDamage(actor, damageable);
                }
            }
        }

        /// <summary>
        /// Apply hazard damage to a specific actor
        /// </summary>
        private void ApplyHazardDamage(GameObject actor, Gameplay.IDamageable damageable)
        {
            float currentTime = Time.time;

            // Check if enough time has passed since last damage
            if (lastDamageTime.TryGetValue(actor, out float lastTime))
            {
                if (currentTime - lastTime < damageTickRate)
                    return;
            }

            // Apply damage
            damageable.TakeDamage(damagePerSecond * damageTickRate, transform.position - actor.transform.position, actor.transform.position);
            lastDamageTime[actor] = currentTime;

#if UNITY_EDITOR || DEVELOPMENT_BUILD
            Debug.Log($"Hazard applied {damagePerSecond * damageTickRate} {GetHazardTypeName()} damage to {actor.name}");
#endif
        }

        private void OnTriggerEnter(Collider other)
        {
            if (!isActive)
                return;

            if (!overlappingActors.Contains(other.gameObject))
            {
                overlappingActors.Add(other.gameObject);
#if UNITY_EDITOR || DEVELOPMENT_BUILD
                Debug.Log($"{other.gameObject.name} entered hazard zone");
#endif
            }
        }

        private void OnTriggerExit(Collider other)
        {
            overlappingActors.Remove(other.gameObject);
            lastDamageTime.Remove(other.gameObject);

#if UNITY_EDITOR || DEVELOPMENT_BUILD
            Debug.Log($"{other.gameObject.name} exited hazard zone");
#endif
        }

        /// <summary>
        /// Create visual effects for the hazard
        /// </summary>
        private void CreateHazardEffects()
        {
            // If particle system is assigned, configure it
            if (hazardEffect != null)
            {
                var main = hazardEffect.main;
                main.startColor = hazardColor;
            }
        }

        /// <summary>
        /// Get human-readable hazard type name
        /// </summary>
        private string GetHazardTypeName()
        {
            return hazardType switch
            {
                HazardType.Thermal => "thermal",
                HazardType.Electrical => "electrical",
                HazardType.Toxic => "toxic",
                HazardType.Radiation => "radiation",
                HazardType.Cryogenic => "cryogenic",
                _ => "unknown"
            };
        }

        // Public setters for game manager
        public void SetHazardType(HazardType type) => hazardType = type;
        public void SetDamagePerSecond(float damage) => damagePerSecond = damage;
        public void SetEffectRadius(float radius)
        {
            effectRadius = radius;
            if (hazardVolume != null)
                hazardVolume.radius = radius;
        }

        private void OnDrawGizmos()
        {
            Gizmos.color = new Color(hazardColor.r, hazardColor.g, hazardColor.b, 0.3f);
            Gizmos.DrawSphere(transform.position, effectRadius);
        }
    }
}
