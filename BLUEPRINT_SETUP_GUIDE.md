# Blueprint Setup & Configuration Guide

## What You Need to Do in Unreal Editor

After compiling the C++ code, follow these steps to get the game fully playable.

---

## Step 1: Create Blueprint Classes

### Create BP_NeonEnemy

1. Open Content Browser
2. Right-click → **Blueprint Class**
3. Search for and select **ANeonEnemy**
4. Name it **`BP_NeonEnemy`**
5. Open the blueprint and configure:

   **Details Panel:**
   - **Mesh** section:
     - Skeletal Mesh: (Set to humanoid mesh or leave as default capsule)
   - **Pawn** section:
     - AI Controller Class: **ANeonEnemyController**
   - **Combat** section:
     - Weapon Class: **BP_AssaultRifle** (or any ANeonWeapon subclass)
     - Detection Range: 2000
     - Attack Range: 500
   - **AI|Detection** section:
     - Detection Range: 2000
     - Lost Target Distance: 3000
   - **AI|Patrol** section:
     - Patrol Speed: 300
   - **AI|Retreat** section:
     - Retreat Health Threshold: 0.25

6. Save and compile

### Create BP_DistrictHazard (Optional)

1. Right-click → **Blueprint Class**
2. Select **ADistrictHazard**
3. Name it **`BP_DistrictHazard`**
4. Open and configure:

   **Details Panel:**
   - **Hazard** section:
     - Hazard Type: (Leave as default or set to preference)
     - Damage Per Second: 10
     - Effect Radius: 500
     - Is Active: True
   - **Visual** section:
     - Hazard Color: (Will be overridden by hazard type)
     - Visual Intensity: 1.0

5. Save and compile

### Create BP_NeonGameMode

1. Right-click → **Blueprint Class**
2. Select **ANeonGameMode**
3. Name it **`BP_NeonGameMode`**
4. Open and configure:

   **Details Panel → Mission section:**
   - **Enemy Class**: `BP_NeonEnemy` (the blueprint you just created)
   - **Hazard Class**: `BP_DistrictHazard` (optional, can leave blank)

5. Save and compile

---

## Step 2: Update Level Settings

### Set Default Game Mode

1. Open your test level (e.g., `TestLevel`)
2. Click **World Settings** (top right of viewport)
3. Find **Game Mode** section:
   - **Game Mode Override**: Set to **BP_NeonGameMode**

### Ensure NavMesh is Built

1. Select all floor/walkable geometry
2. In Details → Search for "Nav"
   - Set **Generate Nav Mesh** to True
3. Click **Build** menu → **Build Paths**
   - Green overlay will appear showing valid navigation areas
4. If no green overlay, check:
   - Geometry has proper collision
   - NavMesh Bounds Volume is present
   - "Generate Nav Mesh" is enabled

### Place NavMesh Bounds Volume (if not present)

1. Content Browser → Search "NavMesh"
2. Drag **NavMeshBoundsVolume** into level
3. Scale it to cover all walkable areas
4. Click **Build** → **Build Paths**

---

## Step 3: Create Test Weapons (If Needed)

If you don't have `BP_AssaultRifle` already created:

1. Right-click in Content Browser → **Blueprint Class**
2. Select **ANeonWeapon**
3. Name it **`BP_AssaultRifle`**
4. Configure:
   - **Weapon** section:
     - Damage: 25
     - Fire Rate: 0.1 (10 shots/second)
     - Max Ammo: 30
     - Current Ammo: 30
     - Is Automatic: True
5. Save

---

## Step 4: Test the Setup

### Manual Testing

1. **Start Gameplay:**
   - Press **Play** in editor
   - Player should spawn and appear in level

2. **Test Movement:**
   - Press **W/A/S/D** to move
   - Move **Mouse** to look around
   - Press **Space** to jump
   - Hold **Shift** to sprint

3. **Test HUD:**
   - Look top-left: **Health bar** should display (green)
   - Look bottom-left: **Ammo counter** should show "Ammo: 30 / 30"
   - Look left side: **Mission briefing** should show:
     - District: [Some district name]
     - Opposition: [Some faction]
     - Etc.

4. **Test Enemies:**
   - Enemies should be visible in the level
   - Approach enemy: it should chase you
   - Get close: it should start firing
   - If enemies don't spawn:
     - Check BP_NeonGameMode → Enemy Class is set
     - Check NavMesh is built
     - Check console for error messages

5. **Test Combat:**
   - Shoot enemies: **Left Mouse Button**
   - Reload: **R** key
   - Enemies should take damage (health decreases)
   - When enemy health = 0: it should ragdoll and die

6. **Test Hazards:**
   - Hazards should be visible as sphere zones (optional particles)
   - Walk into hazard: player takes damage
   - Enemy in hazard: enemy takes damage too
   - Health bar should turn yellow/red as player takes damage

---

## Debugging Common Issues

### "No enemies spawn"

**Cause:** `BP_NeonGameMode→Enemy Class` not set

**Fix:**
1. Open `BP_NeonGameMode` blueprint
2. Compile and go to Details
3. Find **Mission** section
4. Set **Enemy Class** → `BP_NeonEnemy`
5. Recompile

### "Enemies spawn but don't move"

**Cause:** NavMesh not built or enemies can't pathfind

**Fix:**
1. Build → Build Paths (Shift+P to visualize)
2. Check that green navigation mesh covers enemy spawn areas
3. Verify geometry has proper collision (WorldDynamic or WorldStatic)
4. Place all geometry BELOW z-height of 0 for proper nav generation

### "Enemies don't fire"

**Cause:** WeaponClass not set on `BP_NeonEnemy`

**Fix:**
1. Open `BP_NeonEnemy`
2. Find **Combat** section
3. Set **Weapon Class** → `BP_AssaultRifle` (or weapon you created)
4. Recompile

### "Player doesn't take damage"

**Cause:** Multiple possibilities - check logs (View → Output Log)

**Fixes:**
- Weapons have Instigator set (should be automatic)
- Check that bullets are hitting player (debug with Draw Debug Lines)
- Player health system active (should be automatic)

### "HUD doesn't display"

**Cause:** `ANeonGameMode` HUD class not set

**Fix:**
1. Verify `ANeonGameMode.cpp` sets `HUDClass = ANeonHUD::StaticClass()`
2. Recompile C++
3. Verify `BP_NeonGameMode` uses default HUD (don't override)

---

## Console Commands for Testing

Open console with **`** (backtick) and type:

```
# Spawn test enemy at camera location
Summon BP_NeonEnemy

# Display AI state machine debug info
Stat Unit  (shows framerate)

# Show navigation mesh
ShowDebug AI

# Damage player (testing)
# (Set up a custom console command if needed)
```

---

## Performance Tips

1. **Enemy AI Tick Rate:** Default 0.2s (5x per second) - adjust if laggy
   - Edit `ANeonEnemyController.cpp` constructor: `PrimaryActorTick.TickInterval`

2. **Hazard Damage Tick Rate:** Default 1.0s - increase if performance issues
   - Edit `ADistrictHazard` Details in blueprint

3. **Enemy Count:** Default 3 per mission - reduce if needed
   - Edit `ANeonGameMode::SpawnEnemiesForMission()` parameter

4. **Hazard Count:** Default 2-3 per mission - reduce if needed
   - Edit `ANeonGameMode::SpawnHazardsForMission()` loop count

---

## Next: Launching New Missions

### From Blueprint

Add a **Custom Event** in `BP_NeonGameMode`:

```
Custom Event: "StartMission"
├─ Get Game Mode (cast to ANeonGameMode)
├─ Call → StartNewMission()
```

Then trigger from:
- **Level Blueprint** → BeginPlay
- **HUD Button Click**
- **Player input** (press a key)

Example in Level Blueprint:
```
Event BeginPlay
│
└─ Delay (1 second, for everything to initialize)
   │
   └─ Get Game Mode (ANeonGameMode)
      │
      └─ Call StartNewMission()
```

---

## What Each System Does (Quick Reference)

| System | What Happens |
|--------|-------------|
| **ANeonGameMode** | Orchestrates missions, spawns enemies/hazards, updates HUD |
| **ANeonEnemy + Controller** | Enemy patrol, chase, fire, take damage, die |
| **ANeonWeapon** | Hitscan shooting (player and enemies) |
| **ADistrictHazard** | Environmental damage zones |
| **ANeonCharacter** | Player movement, shooting, health, death |
| **ANeonHUD** | Displays health, ammo, mission briefing, objectives |

---

## Quick Customization

Want to change how the game plays? Edit these properties:

**In BP_NeonEnemy:**
- Increase `Detection Range` → enemies see further
- Decrease `Fire Interval` → enemies shoot faster
- Increase `Max Health` → enemies harder to kill

**In BP_DistrictHazard:**
- Increase `Damage Per Second` → hazards hurt more
- Increase `Effect Radius` → bigger danger zones

**In ANeonGameMode.cpp:**
- Change `EnemyCount` parameter in `SpawnEnemiesForMission()` call
- Change `2, 3` in `FMath::RandRange(2, 3)` for hazard count

**In ANeonCharacter:**
- Increase `MaxHealth` → player takes more damage
- Decrease `Fire Rate` in weapon → faster shooting

---

## Final Checklist

- [ ] Compiled C++ code (no errors)
- [ ] Created `BP_NeonEnemy` with WeaponClass set
- [ ] Created `BP_NeonGameMode` with Enemy Class set
- [ ] Created `BP_AssaultRifle` weapon
- [ ] Set World Settings → Game Mode to `BP_NeonGameMode`
- [ ] Built NavMesh (green overlay visible)
- [ ] Placed walkable geometry in level
- [ ] Tested Play mode
- [ ] Enemies spawn and move
- [ ] Player can shoot enemies
- [ ] Enemies shoot back
- [ ] HUD displays correctly
- [ ] Hazards damage player and enemies

Once all checkboxes are done, you have a fully playable mission!

---

## Troubleshooting Support

If something doesn't work:

1. **Check Output Log** (View → Output Log):
   - Look for error messages
   - Post error text to get help

2. **Verify Compilation**:
   - File → Compile
   - File → Refresh
   - If red X on blueprint, click it to see errors

3. **Check Blueprint Settings**:
   - Double-click the blueprint
   - Click "Compile" button
   - Check for validation errors (red warning icons)

4. **Delete Intermediate Files**:
   - Close Unreal Editor
   - Delete `Intermediate/` folder
   - Reopen project (will regenerate)
