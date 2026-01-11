using System.Collections;
using UnityEngine;

namespace NeonAscendant.Gameplay
{
    /// <summary>
    /// Hitscan weapon system with ammo, reloading, and auto/semi-auto modes
    /// </summary>
    public class NeonWeapon : MonoBehaviour
    {
        [Header("Components")]
        [SerializeField] private Transform muzzleLocation;
        [SerializeField] private GameObject weaponMesh;

        [Header("Weapon Stats")]
        [SerializeField] private float damage = 20.0f;
        [SerializeField] private float fireRate = 0.1f;
        [SerializeField] private float range = 10000.0f;
        [SerializeField] private int maxAmmo = 30;
        [SerializeField] private float reloadTime = 2.0f;
        [SerializeField] private bool isAutomatic = true;

        [Header("Weapon State")]
        [SerializeField] private int currentAmmo;
        [SerializeField] private bool isFiring = false;
        [SerializeField] private bool isReloading = false;

        private Coroutine fireCoroutine;
        private Coroutine reloadCoroutine;
        private Camera playerCamera;

        // Public getters
        public int CurrentAmmo => currentAmmo;
        public int MaxAmmo => maxAmmo;
        public bool IsReloading => isReloading;

        private void Start()
        {
            // Initialize ammo
            currentAmmo = maxAmmo;

            // Cache camera reference
            playerCamera = Camera.main;
        }

        /// <summary>
        /// Start firing the weapon
        /// </summary>
        public void StartFire()
        {
            if (isReloading)
                return;

            isFiring = true;

            if (isAutomatic)
            {
                // Automatic fire
                if (fireCoroutine != null)
                    StopCoroutine(fireCoroutine);
                fireCoroutine = StartCoroutine(AutomaticFireCoroutine());
            }
            else
            {
                // Semi-automatic fire
                Fire();
            }
        }

        /// <summary>
        /// Stop firing the weapon
        /// </summary>
        public void StopFire()
        {
            isFiring = false;

            if (fireCoroutine != null)
            {
                StopCoroutine(fireCoroutine);
                fireCoroutine = null;
            }
        }

        /// <summary>
        /// Automatic fire coroutine
        /// </summary>
        private IEnumerator AutomaticFireCoroutine()
        {
            Fire();
            while (isFiring)
            {
                yield return new WaitForSeconds(fireRate);
                Fire();
            }
        }

        /// <summary>
        /// Fire a single shot
        /// </summary>
        private void Fire()
        {
            if (currentAmmo <= 0)
            {
                StopFire();
                return;
            }

            currentAmmo--;

            if (playerCamera == null)
            {
                playerCamera = Camera.main;
                if (playerCamera == null)
                    return;
            }

            // Get camera viewpoint for accurate shooting
            Vector3 cameraLocation;
            Quaternion cameraRotation;
            if (!GetCameraViewpoint(out cameraLocation, out cameraRotation))
                return;

            // Calculate shot direction from camera
            Vector3 shotDirection = cameraRotation * Vector3.forward;
            Vector3 traceStart = cameraLocation;
            Vector3 traceEnd = traceStart + (shotDirection * range);

            // Perform raycast
            RaycastHit hitResult;
            bool hit = PerformRaycast(traceStart, shotDirection, range, out hitResult);

            if (hit)
            {
                ApplyDamageToHit(hitResult, shotDirection);
            }

            // Debug visualization
            DrawDebugShot(traceStart, hit ? hitResult.point : traceEnd, hit);
        }

        /// <summary>
        /// Get camera viewpoint
        /// </summary>
        private bool GetCameraViewpoint(out Vector3 location, out Quaternion rotation)
        {
            location = Vector3.zero;
            rotation = Quaternion.identity;

            if (playerCamera == null)
                return false;

            location = playerCamera.transform.position;
            rotation = playerCamera.transform.rotation;
            return true;
        }

        /// <summary>
        /// Perform raycast
        /// </summary>
        private bool PerformRaycast(Vector3 start, Vector3 direction, float distance, out RaycastHit hit)
        {
            // Ignore self and owner
            int layerMask = ~(1 << gameObject.layer);

            return Physics.Raycast(start, direction, out hit, distance, layerMask);
        }

        /// <summary>
        /// Apply damage to hit target
        /// </summary>
        private void ApplyDamageToHit(RaycastHit hitResult, Vector3 shotDirection)
        {
            if (hitResult.collider == null)
                return;

            // Try to apply damage to hit object
            var damageable = hitResult.collider.GetComponent<IDamageable>();
            if (damageable != null)
            {
                damageable.TakeDamage(damage, shotDirection, hitResult.point);
            }
        }

        /// <summary>
        /// Draw debug visualization for shot
        /// </summary>
        private void DrawDebugShot(Vector3 start, Vector3 end, bool hit)
        {
#if UNITY_EDITOR
            if (hit)
            {
                Debug.DrawLine(start, end, Color.red, 1.0f);
            }
            else
            {
                Debug.DrawLine(start, end, Color.white, 1.0f);
            }
#endif
        }

        /// <summary>
        /// Reload the weapon
        /// </summary>
        public void Reload()
        {
            if (isReloading || currentAmmo == maxAmmo)
                return;

            if (reloadCoroutine != null)
                StopCoroutine(reloadCoroutine);

            reloadCoroutine = StartCoroutine(ReloadCoroutine());
        }

        /// <summary>
        /// Reload coroutine
        /// </summary>
        private IEnumerator ReloadCoroutine()
        {
            isReloading = true;
            StopFire();

            yield return new WaitForSeconds(reloadTime);

            FinishReload();
        }

        /// <summary>
        /// Finish reloading
        /// </summary>
        private void FinishReload()
        {
            isReloading = false;
            currentAmmo = maxAmmo;
        }
    }

    /// <summary>
    /// Interface for objects that can take damage
    /// </summary>
    public interface IDamageable
    {
        void TakeDamage(float damage, Vector3 direction, Vector3 hitPoint);
    }
}
