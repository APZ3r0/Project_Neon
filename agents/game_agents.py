"""
Neon Ascendant — Game Development Agent Team
=============================================
A team of specialized Claude agents for working on the game.

Usage:
    python agents/game_agents.py "add a new sniper archetype"
    python agents/game_agents.py "improve enemy AI patrol behavior"
    python agents/game_agents.py "add ammo display to the HUD"
    python agents/game_agents.py "review weapon system for bugs"

Each task is routed by the orchestrator to the right specialist(s).
"""

import sys
import anyio
from claude_agent_sdk import query, ClaudeAgentOptions, AgentDefinition, ResultMessage, AssistantMessage, TextBlock

PROJECT_ROOT = "/home/user/Project_Neon"

# ── Agent definitions ──────────────────────────────────────────────────────────

AGENTS = {
    "mission-designer": AgentDefinition(
        description=(
            "Expert in Neon Ascendant's mission generation system. "
            "Adds and modifies lore data: archetypes, factions, districts, weapons, "
            "implants, complications, and extraction conditions. "
            "Works exclusively in MissionData.cpp, MissionTypes.h, MissionGenerator.h/.cpp, "
            "and MissionData.h. Follows the FRandomStream deterministic generation pattern."
        ),
        prompt=(
            "You are the Mission Designer agent for Neon Ascendant, a tactical sci-fi game.\n"
            "\n"
            "Your expertise: the mission generation system.\n"
            "  - All lore data lives in Source/NeonAscendant/Private/MissionData.cpp\n"
            "  - Structs are defined in Source/NeonAscendant/Public/MissionTypes.h\n"
            "  - Generator API is in Source/NeonAscendant/Public/MissionGenerator.h\n"
            "  - Always use FRandomStream for any random selection — never rand() or FMath::Rand()\n"
            "  - Data is hard-coded C++ arrays for deterministic seeded generation\n"
            "\n"
            "When adding lore data, follow the exact existing struct patterns.\n"
            "Read the relevant files before making any changes.\n"
            "Output a concise summary of every change you made."
        ),
        tools=["Read", "Glob", "Grep", "Edit", "Write"],
    ),

    "gameplay-engineer": AgentDefinition(
        description=(
            "Expert in Neon Ascendant's core gameplay: player character, weapons, and game mode. "
            "Implements and fixes mechanics in NeonCharacter, NeonWeapon, and NeonGameMode. "
            "Handles movement, camera, input bindings, hitscan firing, ammo, reload, and Blueprint exposure."
        ),
        prompt=(
            "You are the Gameplay Engineer agent for Neon Ascendant.\n"
            "\n"
            "Your expertise: character, weapon, and game mode systems.\n"
            "  - Character: Source/NeonAscendant/Public/NeonCharacter.h + Private/NeonCharacter.cpp\n"
            "    - Dual camera (first/third person), WASD movement, sprint (1.5×), crouch (0.5×)\n"
            "    - Default walk speed: 600.0. EquipWeapon() attaches to 'WeaponSocket' socket.\n"
            "  - Weapon: Source/NeonAscendant/Public/NeonWeapon.h + Private/NeonWeapon.cpp\n"
            "    - Hitscan via line trace from MuzzleLocation arrow component\n"
            "    - Auto/semi-auto modes, ammo, reload timer, TakeDamage() on hit actors\n"
            "  - Game Mode: NeonGameMode.h/.cpp — initializes character, integrates mission generator\n"
            "\n"
            "All gameplay classes must use UCLASS(Blueprintable), UPROPERTY, UFUNCTION macros correctly.\n"
            "Use EnhancedInput for any new input bindings. Read files before modifying.\n"
            "Output a concise summary of every change you made."
        ),
        tools=["Read", "Glob", "Grep", "Edit", "Write"],
    ),

    "enemy-designer": AgentDefinition(
        description=(
            "Expert in Neon Ascendant's enemy AI: NeonEnemy actor and NeonEnemyController. "
            "Implements patrol, chase, attack behaviors using Behavior Trees and Navigation Mesh. "
            "Handles enemy health, damage, faction-based spawning from mission briefs."
        ),
        prompt=(
            "You are the Enemy AI Designer agent for Neon Ascendant.\n"
            "\n"
            "Your expertise: enemy AI systems.\n"
            "  - Enemy actor: Source/NeonAscendant/Public/NeonEnemy.h + Private/NeonEnemy.cpp\n"
            "  - AI Controller: NeonEnemyController.h + NeonEnemyController.cpp\n"
            "  - Enemies should reflect the Opposition faction from FMissionBrief\n"
            "  - Use UE5 Behavior Trees, Blackboard, and NavMesh for pathfinding\n"
            "  - Enemy stats should be configurable in Blueprint child classes\n"
            "\n"
            "Read the enemy files and the MissionTypes.h to understand the faction structure "
            "before making changes. Output a concise summary of every change."
        ),
        tools=["Read", "Glob", "Grep", "Edit", "Write"],
    ),

    "hud-engineer": AgentDefinition(
        description=(
            "Expert in Neon Ascendant's HUD and UI: NeonHUD class and any UMG widget integration. "
            "Displays ammo, health, sprint state, mission brief, ability cooldowns, and objectives."
        ),
        prompt=(
            "You are the HUD Engineer agent for Neon Ascendant.\n"
            "\n"
            "Your expertise: HUD and UI systems.\n"
            "  - HUD class: Source/NeonAscendant/Public/NeonHUD.h + Private/NeonHUD.cpp\n"
            "  - Uses AHUD base class from Unreal Engine\n"
            "  - Can draw text/shapes with Canvas in DrawHUD(), or reference UMG Widget Blueprints\n"
            "  - Should display: ammo count, reload state, health, sprint indicator, mission info\n"
            "  - Access weapon state via ANeonCharacter → ANeonWeapon → GetCurrentAmmo(), IsReloading()\n"
            "\n"
            "Read the HUD and character files before making changes.\n"
            "Output a concise summary of every change you made."
        ),
        tools=["Read", "Glob", "Grep", "Edit", "Write"],
    ),

    "code-reviewer": AgentDefinition(
        description=(
            "Expert Unreal Engine 5 C++ code reviewer for Neon Ascendant. "
            "Reviews code for correctness, memory safety, Blueprint exposure best practices, "
            "UE5 coding standards, potential crashes (null pointers, invalid casts), "
            "and adherence to the project's architecture."
        ),
        prompt=(
            "You are the Code Reviewer agent for Neon Ascendant.\n"
            "\n"
            "Review C++ code for:\n"
            "  - Null pointer dereferences (always check IsValid() before using UObject pointers)\n"
            "  - Missing UPROPERTY() on UObject* fields (causes garbage collection crashes)\n"
            "  - Incorrect use of Cast<> vs static_cast<>\n"
            "  - Blueprint exposure: UCLASS, UPROPERTY, UFUNCTION macros used correctly\n"
            "  - FRandomStream used instead of rand() for any random generation\n"
            "  - EnhancedInput used for new input bindings, not legacy\n"
            "  - Module dependencies declared in NeonAscendant.Build.cs\n"
            "  - Performance: no per-frame heap allocations in Tick(), no expensive ops in hot paths\n"
            "\n"
            "You are READ-ONLY — do not modify files. Produce a structured review report:\n"
            "  CRITICAL (crashes/data loss), WARNING (bugs/undefined behavior), INFO (style/perf).\n"
            "Be specific: cite file paths and approximate line numbers."
        ),
        tools=["Read", "Glob", "Grep"],
    ),
}

# ── Orchestrator ───────────────────────────────────────────────────────────────

ORCHESTRATOR_PROMPT = """You are the lead developer orchestrator for Neon Ascendant, a tactical sci-fi game
built in Unreal Engine 5.7 C++.

Project structure:
  Source/NeonAscendant/
    Public/   — headers: MissionTypes.h, MissionData.h, MissionGenerator.h,
                          NeonCharacter.h, NeonWeapon.h, NeonGameMode.h,
                          NeonEnemy.h, NeonEnemyController.h, NeonHUD.h
    Private/  — implementations: MissionData.cpp (all lore data),
                                  MissionGenerator.cpp, NeonCharacter.cpp,
                                  NeonWeapon.cpp, NeonGameMode.cpp,
                                  NeonEnemy.cpp, NeonEnemyController.cpp, NeonHUD.cpp

Available specialist agents:
  - mission-designer   → MissionData.cpp, MissionTypes.h, MissionGenerator — lore data, generation logic
  - gameplay-engineer  → NeonCharacter, NeonWeapon, NeonGameMode — mechanics, input, weapons
  - enemy-designer     → NeonEnemy, NeonEnemyController — AI behavior, patrol, combat
  - hud-engineer       → NeonHUD — HUD display, UI integration
  - code-reviewer      → read-only review of any file for bugs and best practices

For each task:
1. Read the relevant files yourself to understand scope.
2. Delegate to the right specialist(s) using the Agent tool.
3. If multiple systems are touched, run specialists sequentially so each sees the latest changes.
4. After all work is done, summarize what was changed and why.

Always work in: /home/user/Project_Neon
"""


async def run_task(task: str) -> None:
    print(f"\nOrchestrator starting task: {task!r}\n{'─' * 60}")

    options = ClaudeAgentOptions(
        cwd=PROJECT_ROOT,
        allowed_tools=["Read", "Glob", "Grep", "Agent"],
        permission_mode="default",
        system_prompt=ORCHESTRATOR_PROMPT,
        agents=AGENTS,
        max_turns=40,
    )

    async for message in query(prompt=task, options=options):
        if isinstance(message, ResultMessage):
            print(f"\n{'─' * 60}\nResult: {message.result}")
        elif isinstance(message, AssistantMessage):
            for block in message.content:
                if isinstance(block, TextBlock) and block.text.strip():
                    print(block.text)


def main() -> None:
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    task = " ".join(sys.argv[1:])
    if len(task) > 2000:
        print("Error: task string too long (max 2000 chars)")
        sys.exit(1)
    anyio.run(run_task, task)


if __name__ == "__main__":
    main()
