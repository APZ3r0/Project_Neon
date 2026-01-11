using UnityEngine;
using UnityEngine.UI;
using TMPro;
using NeonAscendant.Gameplay;
using NeonAscendant.Mission;
using NeonAscendant.Core;

namespace NeonAscendant.UI
{
    /// <summary>
    /// HUD system displaying health, ammo, and mission information
    /// </summary>
    public class NeonHUD : MonoBehaviour
    {
        [Header("References")]
        [SerializeField] private NeonCharacter playerCharacter;

        [Header("UI Elements - Health")]
        [SerializeField] private Image healthBarFill;
        [SerializeField] private TextMeshProUGUI healthText;

        [Header("UI Elements - Ammo")]
        [SerializeField] private TextMeshProUGUI ammoText;

        [Header("UI Elements - Mission")]
        [SerializeField] private TextMeshProUGUI missionBriefingText;
        [SerializeField] private TextMeshProUGUI objectiveText;

        // Health color thresholds
        private const float HealthDamagedThreshold = 0.5f;
        private const float HealthCriticalThreshold = 0.25f;

        private void Start()
        {
            // Auto-find player if not assigned
            if (playerCharacter == null)
            {
                var playerObj = GameObject.FindGameObjectWithTag("Player");
                if (playerObj != null)
                {
                    playerCharacter = playerObj.GetComponent<NeonCharacter>();
                }
            }

            UpdateMissionBriefing();
        }

        private void Update()
        {
            if (playerCharacter == null)
                return;

            UpdateHealthBar();
            UpdateAmmoCounter();
        }

        /// <summary>
        /// Update health bar display
        /// </summary>
        private void UpdateHealthBar()
        {
            float healthPercent = playerCharacter.HealthPercent;

            if (healthBarFill != null)
            {
                healthBarFill.fillAmount = healthPercent;
                healthBarFill.color = GetHealthColor(healthPercent);
            }

            if (healthText != null)
            {
                healthText.text = $"HP: {playerCharacter.Health:F0} / {playerCharacter.MaxHealth:F0}";
            }
        }

        /// <summary>
        /// Get health bar color based on health percentage
        /// </summary>
        private Color GetHealthColor(float healthPercent)
        {
            if (healthPercent >= HealthDamagedThreshold)
                return Color.green;
            else if (healthPercent >= HealthCriticalThreshold)
                return Color.yellow;
            else
                return Color.red;
        }

        /// <summary>
        /// Update ammo counter display
        /// </summary>
        private void UpdateAmmoCounter()
        {
            if (ammoText == null)
                return;

            var weapon = playerCharacter.CurrentWeapon;
            if (weapon != null)
            {
                if (weapon.IsReloading)
                {
                    ammoText.text = "RELOADING...";
                    ammoText.color = Color.yellow;
                }
                else
                {
                    ammoText.text = $"AMMO: {weapon.CurrentAmmo} / {weapon.MaxAmmo}";
                    ammoText.color = weapon.CurrentAmmo > 0 ? Color.white : Color.red;
                }
            }
            else
            {
                ammoText.text = "NO WEAPON";
                ammoText.color = Color.gray;
            }
        }

        /// <summary>
        /// Update mission briefing display
        /// </summary>
        private void UpdateMissionBriefing()
        {
            if (missionBriefingText == null)
                return;

            var mission = NeonGameManager.Instance?.CurrentMission;
            if (mission.HasValue)
            {
                var m = mission.Value;
                string briefing = $"MISSION BRIEF\n" +
                                $"District: {m.District.Name}\n" +
                                $"Opposition: {m.Opposition.Name}\n" +
                                $"Archetype: {m.Archetype.Name}\n" +
                                $"Weapon: {m.PrimaryWeapon.Name}\n" +
                                $"Implant: {m.BackupImplant.Name}\n" +
                                $"Complication: {m.Complication}";

                missionBriefingText.text = briefing;
            }
            else
            {
                missionBriefingText.text = "NO ACTIVE MISSION";
            }

            UpdateObjectiveTracker();
        }

        /// <summary>
        /// Update objective tracker
        /// </summary>
        private void UpdateObjectiveTracker()
        {
            if (objectiveText == null)
                return;

            var mission = NeonGameManager.Instance?.CurrentMission;
            if (mission.HasValue)
            {
                objectiveText.text = $"OBJECTIVE: {mission.Value.ExtractionCondition}";
            }
            else
            {
                objectiveText.text = "";
            }
        }

        /// <summary>
        /// Set player character reference
        /// </summary>
        public void SetPlayerCharacter(NeonCharacter newPlayer)
        {
            playerCharacter = newPlayer;
        }

        /// <summary>
        /// Refresh mission display (call when new mission starts)
        /// </summary>
        public void RefreshMissionDisplay()
        {
            UpdateMissionBriefing();
        }
    }
}
