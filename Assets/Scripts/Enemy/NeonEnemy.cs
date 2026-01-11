using UnityEngine;
using UnityEngine.AI;

namespace NeonAscendant.Enemy
{
    /// <summary>
    /// Basic enemy AI with health and damage system
    /// </summary>
    [RequireComponent(typeof(NavMeshAgent))]
    public class NeonEnemy : MonoBehaviour, Gameplay.IDamageable
    {
        [Header("Health")]
        [SerializeField] private float maxHealth = 100.0f;
        [SerializeField] private float currentHealth;

        [Header("AI Behavior")]
        [SerializeField] private float detectionRange = 15.0f;
        [SerializeField] private float attackRange = 10.0f;
        [SerializeField] private float wanderRadius = 10.0f;

        [Header("Movement")]
        [SerializeField] private float moveSpeed = 3.5f;
        [SerializeField] private float rotationSpeed = 500.0f;

        private NavMeshAgent agent;
        private GameObject player;
        private bool isDead = false;

        private const float TickInterval = 0.1f;
        private float tickTimer = 0f;
        private const float DeathLifespan = 10.0f;

        // Public getters
        public float Health => currentHealth;
        public float MaxHealth => maxHealth;
        public float HealthPercent => maxHealth > 0.0f ? currentHealth / maxHealth : 0.0f;

        private void Awake()
        {
            agent = GetComponent<NavMeshAgent>();
            agent.speed = moveSpeed;
        }

        private void Start()
        {
            currentHealth = maxHealth;

            // Find player
            var playerObj = GameObject.FindGameObjectWithTag("Player");
            if (playerObj != null)
            {
                player = playerObj;
            }
        }

        private void Update()
        {
            if (isDead)
                return;

            tickTimer += Time.deltaTime;
            if (tickTimer >= TickInterval)
            {
                tickTimer = 0f;
                UpdateAI();
            }
        }

        /// <summary>
        /// Update AI behavior
        /// </summary>
        private void UpdateAI()
        {
            if (player == null)
                return;

            float distanceToPlayer = Vector3.Distance(transform.position, player.transform.position);

            if (distanceToPlayer <= detectionRange)
            {
                // Chase player
                agent.SetDestination(player.transform.position);

                // Face player
                Vector3 direction = (player.transform.position - transform.position).normalized;
                direction.y = 0;
                if (direction != Vector3.zero)
                {
                    Quaternion lookRotation = Quaternion.LookRotation(direction);
                    transform.rotation = Quaternion.RotateTowards(transform.rotation, lookRotation, rotationSpeed * Time.deltaTime);
                }

                // Attack if in range
                if (distanceToPlayer <= attackRange)
                {
                    // TODO: Implement shooting behavior when weapon system is integrated
                }
            }
            else
            {
                // Wander randomly
                if (!agent.hasPath || agent.remainingDistance < 0.5f)
                {
                    Vector3 randomDirection = Random.insideUnitSphere * wanderRadius;
                    randomDirection += transform.position;
                    randomDirection.y = transform.position.y;

                    if (NavMesh.SamplePosition(randomDirection, out NavMeshHit hit, wanderRadius, NavMesh.AllAreas))
                    {
                        agent.SetDestination(hit.position);
                    }
                }
            }
        }

        /// <summary>
        /// Take damage
        /// </summary>
        public void TakeDamage(float damage, Vector3 direction, Vector3 hitPoint)
        {
            if (isDead)
                return;

            float actualDamage = Mathf.Max(damage, 0f);
            if (actualDamage <= 0f)
                return;

            currentHealth -= actualDamage;

#if UNITY_EDITOR || DEVELOPMENT_BUILD
            Debug.Log($"Enemy took {actualDamage} damage. Health: {currentHealth}/{maxHealth}");
#endif

            if (currentHealth <= 0.0f)
            {
                currentHealth = 0.0f;
                Die();
            }
        }

        /// <summary>
        /// Handle enemy death
        /// </summary>
        private void Die()
        {
            if (isDead)
                return;

            isDead = true;

#if UNITY_EDITOR || DEVELOPMENT_BUILD
            Debug.Log($"Enemy {gameObject.name} died");
#endif

            // Disable AI
            if (agent != null)
            {
                agent.enabled = false;
            }

            // Disable colliders
            var colliders = GetComponentsInChildren<Collider>();
            foreach (var col in colliders)
            {
                col.enabled = false;
            }

            // Destroy after delay
            Destroy(gameObject, DeathLifespan);
        }

        private void OnDrawGizmosSelected()
        {
            // Draw detection range
            Gizmos.color = Color.yellow;
            Gizmos.DrawWireSphere(transform.position, detectionRange);

            // Draw attack range
            Gizmos.color = Color.red;
            Gizmos.DrawWireSphere(transform.position, attackRange);
        }
    }
}
