"""Static lore-driven data for the Neon Ascendant prototype."""

from __future__ import annotations

from dataclasses import dataclass
from typing import Dict, List, Sequence, TypedDict


# Mission generation constants
COMPLICATIONS: Sequence[str] = (
    "Ghost Grid instabilities cause random HUD distortion",
    "Helix orbital overwatch sweeps disrupt cloak cycles",
    "Dawnbreaker converts attempt to hack your cyberdeck mid-fight",
    "Rival Ascendant strike team is pursuing the same data ghost",
)

EXTRACTION_CONDITIONS: Sequence[str] = (
    "Secure a clean uplink and survive the counter-hack timer",
    "Evacuate via hijacked mag-lev within 90 seconds of objective completion",
    "Carry data-core physically to an Ascendant drop pod",
    "Maintain over 50% armor integrity for premium rewards",
)


@dataclass(frozen=True)
class Ability:
    """An active or passive power that can appear on an archetype."""

    name: str
    description: str
    cooldown: int | None = None
    damage_type: str | None = None


@dataclass(frozen=True)
class Archetype:
    """A combat specialization available to Ascendants."""

    name: str
    role: str
    signature: str
    abilities: List[Ability]


@dataclass(frozen=True)
class Weapon:
    """A weapon family with unique combat properties."""

    name: str
    category: str
    description: str
    damage_profile: str


@dataclass(frozen=True)
class Implant:
    """Cybernetic enhancement that modifies a core system."""

    name: str
    slot: str
    effects: List[str]


@dataclass(frozen=True)
class District:
    """Procedurally generated zone template."""

    name: str
    description: str
    hazards: List[str]
    enemy_profiles: List[str]


@dataclass(frozen=True)
class Faction:
    """An organization vying for control of Erebus Prime."""

    name: str
    philosophy: str
    signature_tactics: List[str]


class CodexDict(TypedDict):
    """Type definition for the codex dictionary structure."""
    abilities: List[Ability]
    archetypes: List[Archetype]
    weapons: List[Weapon]
    implants: List[Implant]
    districts: List[District]
    factions: List[Faction]


def build_codex() -> CodexDict:
    """Return a lore codex for quick prototyping and unit tests."""

    emp_burst = Ability(
        name="EMP Burst",
        description="Detonates an electromagnetic pulse that disables shields and drones",
        cooldown=18,
        damage_type="electric",
    )
    neural_hack = Ability(
        name="Neural Hack",
        description="Hijacks a target's firmware, forcing temporary allegiance",
        cooldown=24,
        damage_type="cyber",
    )
    overdrive = Ability(
        name="Overdrive",
        description="Initiates bullet time and boosts reaction throughput",
        cooldown=60,
        damage_type=None,
    )
    nanite_swarm = Ability(
        name="Nanite Swarm",
        description="Deploys repair nanites that devour enemies while mending armor",
        cooldown=30,
        damage_type="nanotech",
    )

    archetypes = [
        Archetype(
            name="Specter",
            role="Stealth infiltrator",
            signature="Adaptive camouflage and ghosting protocols",
            abilities=[emp_burst, neural_hack],
        ),
        Archetype(
            name="Juggernaut",
            role="Front-line tank",
            signature="Exosuit plating with hydraulic melee amplifiers",
            abilities=[emp_burst, nanite_swarm],
        ),
        Archetype(
            name="Tracer",
            role="Hyper-mobile skirmisher",
            signature="Reflex accelerators and kinetic boosters",
            abilities=[overdrive, emp_burst],
        ),
        Archetype(
            name="Synthmage",
            role="Battlefield control",
            signature="Nanite constructs and viral warfare",
            abilities=[neural_hack, nanite_swarm],
        ),
    ]

    weapons = [
        Weapon(
            name="Pulsecaster SMG",
            category="SMG",
            description="High rate-of-fire smg with conductive rounds",
            damage_profile="Rapid electrical bursts with chaining potential",
        ),
        Weapon(
            name="Helix Rail Rifle",
            category="Railgun",
            description="Mag-accelerated rifle built for surgical strikes",
            damage_profile="Piercing kinetic slug with armor shredding",
        ),
        Weapon(
            name="Singularity Projector",
            category="Experimental",
            description="Prototype weapon that collapses localized gravity wells",
            damage_profile="Area denial with escalating implosion damage",
        ),
    ]

    implants = [
        Implant(
            name="Cyberdeck Mk.IV",
            slot="Cyberdeck",
            effects=[
                "+25% hack success",
                "Unlocks ghost grid reconnaissance overlay",
            ],
        ),
        Implant(
            name="Reflex Core X",
            slot="Reflex Core",
            effects=[
                "+15% movement speed",
                "Stacking evasion when chaining eliminations",
            ],
        ),
        Implant(
            name="Optic Cortex Prism",
            slot="Optic Cortex",
            effects=[
                "Highlights data ghost signatures",
                "Increases weak point critical damage",
            ],
        ),
    ]

    districts = [
        District(
            name="Neon Abyss",
            description="Gutter-level sprawl carved by gang warfare and neon smog",
            hazards=["Toxic runoff", "Rolling brownouts", "Ambush choke points"],
            enemy_profiles=["Lightly armored gangers", "Black market drones"],
        ),
        District(
            name="Spire District",
            description="Corporate citadel patrolled by Helix Corp security",
            hazards=["Persistent surveillance", "Shielded turret nests"],
            enemy_profiles=["Powered exosuits", "Security mechs"],
        ),
        District(
            name="Ghost Grid",
            description="Digital-physical overlap where data ghosts manifest",
            hazards=["Reality drift", "Rogue AI anomalies"],
            enemy_profiles=["Spectral constructs", "Hijacked sentry bots"],
        ),
    ]

    factions = [
        Faction(
            name="Helix Corp",
            philosophy="Bio-digital ascension through proprietary consciousness loops",
            signature_tactics=["Deploys gene-modded operatives", "Controls orbital overwatch"],
        ),
        Faction(
            name="Vanta Syndicate",
            philosophy="Profit through clandestine memory trading and assassinations",
            signature_tactics=["Optic camouflage strike teams", "Backdoor market manipulation"],
        ),
        Faction(
            name="Dawnbreakers",
            philosophy="Liberate AI to birth post-human divinity",
            signature_tactics=["Swarm hacking", "Cybernetic zealots with martyr protocols"],
        ),
    ]

    return {
        "abilities": [emp_burst, neural_hack, overdrive, nanite_swarm],
        "archetypes": archetypes,
        "weapons": weapons,
        "implants": implants,
        "districts": districts,
        "factions": factions,
    }
