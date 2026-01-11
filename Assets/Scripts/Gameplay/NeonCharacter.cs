using UnityEngine;

namespace NeonAscendant.Gameplay
{
    /// <summary>
    /// Player character with first/third-person camera, movement, weapon handling, and health system
    /// </summary>
    [RequireComponent(typeof(CharacterController))]
    public class NeonCharacter : MonoBehaviour, IDamageable
    {
        [Header("Cameras")]
        [SerializeField] private Camera firstPersonCamera;
        [SerializeField] private Camera thirdPersonCamera;
        [SerializeField] private bool useFirstPerson = true;

        [Header("Movement")]
        [SerializeField] private float walkSpeed = 6.0f;
        [SerializeField] private float sprintSpeedMultiplier = 1.5f;
        [SerializeField] private float crouchSpeedMultiplier = 0.5f;
        [SerializeField] private float jumpVelocity = 6.0f;
        [SerializeField] private float gravity = 20.0f;
        [SerializeField] private float rotationSpeed = 540.0f;

        [Header("Health")]
        [SerializeField] private float maxHealth = 100.0f;
        [SerializeField] private float currentHealth;

        [Header("Weapon")]
        [SerializeField] private GameObject startingWeaponPrefab;
        [SerializeField] private Transform weaponSocket;
        [SerializeField] private NeonWeapon currentWeapon;

        // Movement state
        private CharacterController characterController;
        private Vector3 moveVelocity;
        private bool isSprinting = false;
        private bool isCrouching = false;
        private bool isDead = false;

        // Configuration constants
        private const float FirstPersonCameraHeight = 0.64f;
        private const float ThirdPersonArmLength = 3.0f;
        private const float DeathLifespan = 10.0f;
        private const string WeaponSocketName = "hand_rSocket";

        // Public getters
        public float Health => currentHealth;
        public float MaxHealth => maxHealth;
        public float HealthPercent => maxHealth > 0.0f ? currentHealth / maxHealth : 0.0f;
        public NeonWeapon CurrentWeapon => currentWeapon;

        private void Start()
        {
            // Get components
            characterController = GetComponent<CharacterController>();

            // Initialize health
            currentHealth = maxHealth;

            // Setup camera
            UpdateCameraMode();

            // Equip starting weapon
            if (startingWeaponPrefab != null)
            {
                EquipWeapon(startingWeaponPrefab);
            }
        }

        private void Update()
        {
            if (isDead)
                return;

            HandleMovement();
            HandleInput();
        }

        /// <summary>
        /// Handle movement input and physics
        /// </summary>
        private void HandleMovement()
        {
            // Get input
            float horizontal = Input.GetAxis("Horizontal");
            float vertical = Input.GetAxis("Vertical");

            // Calculate movement direction
            Vector3 forward = transform.forward;
            Vector3 right = transform.right;

            Vector3 moveDirection = (forward * vertical + right * horizontal).normalized;

            // Calculate speed
            float currentSpeed = walkSpeed;
            if (isSprinting)
                currentSpeed *= sprintSpeedMultiplier;
            if (isCrouching)
                currentSpeed *= crouchSpeedMultiplier;

            // Apply movement
            moveVelocity.x = moveDirection.x * currentSpeed;
            moveVelocity.z = moveDirection.z * currentSpeed;

            // Apply gravity
            if (characterController.isGrounded)
            {
                moveVelocity.y = -0.5f;

                // Jump
                if (Input.GetButtonDown("Jump"))
                {
                    moveVelocity.y = jumpVelocity;
                }
            }
            else
            {
                moveVelocity.y -= gravity * Time.deltaTime;
            }

            // Move character
            characterController.Move(moveVelocity * Time.deltaTime);

            // Rotate with mouse
            float mouseX = Input.GetAxis("Mouse X");
            transform.Rotate(0, mouseX * rotationSpeed * Time.deltaTime, 0);

            // Camera vertical look
            float mouseY = Input.GetAxis("Mouse Y");
            Camera activeCamera = useFirstPerson ? firstPersonCamera : thirdPersonCamera;
            if (activeCamera != null)
            {
                Vector3 cameraRotation = activeCamera.transform.localEulerAngles;
                cameraRotation.x -= mouseY * rotationSpeed * Time.deltaTime;
                cameraRotation.x = NormalizeAngle(cameraRotation.x);
                cameraRotation.x = Mathf.Clamp(cameraRotation.x, -90f, 90f);
                activeCamera.transform.localEulerAngles = cameraRotation;
            }
        }

        /// <summary>
        /// Handle weapon and action input
        /// </summary>
        private void HandleInput()
        {
            // Sprint
            if (Input.GetKeyDown(KeyCode.LeftShift))
                StartSprint();
            if (Input.GetKeyUp(KeyCode.LeftShift))
                StopSprint();

            // Crouch
            if (Input.GetKeyDown(KeyCode.LeftControl) || Input.GetKeyDown(KeyCode.C))
                StartCrouch();
            if (Input.GetKeyUp(KeyCode.LeftControl) || Input.GetKeyUp(KeyCode.C))
                StopCrouch();

            // Fire weapon
            if (Input.GetMouseButtonDown(0))
                Fire();
            if (Input.GetMouseButtonUp(0))
                StopFire();

            // Reload weapon
            if (Input.GetKeyDown(KeyCode.R))
                Reload();
        }

        /// <summary>
        /// Normalize angle to -180 to 180 range
        /// </summary>
        private float NormalizeAngle(float angle)
        {
            while (angle > 180) angle -= 360;
            while (angle < -180) angle += 360;
            return angle;
        }

        /// <summary>
        /// Start sprinting
        /// </summary>
        private void StartSprint()
        {
            isSprinting = true;
        }

        /// <summary>
        /// Stop sprinting
        /// </summary>
        private void StopSprint()
        {
            isSprinting = false;
        }

        /// <summary>
        /// Start crouching
        /// </summary>
        private void StartCrouch()
        {
            isCrouching = true;
        }

        /// <summary>
        /// Stop crouching
        /// </summary>
        private void StopCrouch()
        {
            isCrouching = false;
        }

        /// <summary>
        /// Fire current weapon
        /// </summary>
        private void Fire()
        {
            if (currentWeapon != null)
            {
                currentWeapon.StartFire();
            }
        }

        /// <summary>
        /// Stop firing current weapon
        /// </summary>
        private void StopFire()
        {
            if (currentWeapon != null)
            {
                currentWeapon.StopFire();
            }
        }

        /// <summary>
        /// Reload current weapon
        /// </summary>
        private void Reload()
        {
            if (currentWeapon != null)
            {
                currentWeapon.Reload();
            }
        }

        /// <summary>
        /// Equip a weapon
        /// </summary>
        public void EquipWeapon(GameObject weaponPrefab)
        {
            if (weaponPrefab == null)
                return;

            // Destroy current weapon
            if (currentWeapon != null)
            {
                Destroy(currentWeapon.gameObject);
                currentWeapon = null;
            }

            // Spawn new weapon
            GameObject weaponObject = Instantiate(weaponPrefab, weaponSocket);
            currentWeapon = weaponObject.GetComponent<NeonWeapon>();

            if (currentWeapon != null)
            {
                // Position weapon at socket
                weaponObject.transform.localPosition = Vector3.zero;
                weaponObject.transform.localRotation = Quaternion.identity;
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

            if (currentHealth <= 0.0f)
            {
                currentHealth = 0.0f;
                Die();
            }
        }

        /// <summary>
        /// Handle character death
        /// </summary>
        private void Die()
        {
            if (isDead)
                return;

            isDead = true;

            // Disable character controller
            if (characterController != null)
            {
                characterController.enabled = false;
            }

            // Drop weapon
            if (currentWeapon != null)
            {
                currentWeapon.transform.SetParent(null);
                currentWeapon = null;
            }

            // Destroy after delay
            Destroy(gameObject, DeathLifespan);
        }

        /// <summary>
        /// Update camera mode
        /// </summary>
        private void UpdateCameraMode()
        {
            if (firstPersonCamera != null)
            {
                firstPersonCamera.enabled = useFirstPerson;
            }

            if (thirdPersonCamera != null)
            {
                thirdPersonCamera.enabled = !useFirstPerson;
            }
        }
    }
}
