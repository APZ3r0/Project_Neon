"""Export Python codex data to JSON/CSV for Unreal Engine import.

This script exports all game data from the Python prototype to formats
that can be easily imported into Unreal Engine data tables and assets.
"""

import json
import csv
from pathlib import Path
import sys

# Add src to path
sys.path.insert(0, str(Path(__file__).parent.parent / "src"))

from neon_ascendant.data import build_codex
from neon_ascendant.simulation import (
    _ARCHETYPE_PROFILES,
    _ABILITY_SYNERGY,
    _IMPLANT_SYNERGY,
)


def export_codex_json(output_dir: Path) -> None:
    """Export codex data to JSON format."""
    codex = build_codex()
    
    # Merge archetype profiles from simulation.py
    archetype_profiles = _ARCHETYPE_PROFILES
    
    export_data = {
        "archetypes": [
            {
                "name": arch.name,
                "role": arch.role,
                "signature": arch.signature,
                "stats": archetype_profiles.get(arch.name, {
                    "stealth": 0,
                    "assault": 0,
                    "tech": 0,
                    "resilience": 0
                }),
                "abilities": [
                    {
                        "name": a.name,
                        "description": a.description,
                        "cooldown": a.cooldown,
                        "damage_type": a.damage_type
                    }
                    for a in arch.abilities
                ]
            }
            for arch in codex["archetypes"]
        ],
        "weapons": [
            {
                "name": w.name,
                "category": w.category,
                "description": w.description,
                "damage_profile": w.damage_profile
            }
            for w in codex["weapons"]
        ],
        "implants": [
            {
                "name": i.name,
                "slot": i.slot,
                "effects": i.effects
            }
            for i in codex["implants"]
        ],
        "districts": [
            {
                "name": d.name,
                "description": d.description,
                "hazards": d.hazards,
                "enemy_profiles": d.enemy_profiles
            }
            for d in codex["districts"]
        ],
        "factions": [
            {
                "name": f.name,
                "philosophy": f.philosophy,
                "signature_tactics": f.signature_tactics
            }
            for f in codex["factions"]
        ],
        "abilities": [
            {
                "name": a.name,
                "description": a.description,
                "cooldown": a.cooldown,
                "damage_type": a.damage_type,
                "synergy_stat": _ABILITY_SYNERGY.get(a.name)
            }
            for a in codex["abilities"]
        ],
        "constants": {
            "complications": list(codex.get("complications", [])),
            "extraction_conditions": list(codex.get("extraction_conditions", [])),
            "implant_synergy": _IMPLANT_SYNERGY
        }
    }
    
    output_path = output_dir / "codex_export.json"
    with open(output_path, "w", encoding="utf-8") as f:
        json.dump(export_data, f, indent=2, ensure_ascii=False)
    
    print(f"[OK] Exported JSON to {output_path}")
    return export_data


def export_archetypes_csv(data: dict, output_dir: Path) -> None:
    """Export archetypes to CSV for Unreal Data Table."""
    csv_path = output_dir / "Archetypes.csv"
    
    with open(csv_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        # Header row (Unreal Data Table format)
        writer.writerow([
            "Name",
            "Role",
            "Signature",
            "Stealth",
            "Assault",
            "Tech",
            "Resilience",
            "Abilities"
        ])
        
        for arch in data["archetypes"]:
            stats = arch["stats"]
            abilities = ",".join([a["name"] for a in arch["abilities"]])
            writer.writerow([
                arch["name"],
                arch["role"],
                arch["signature"],
                stats["stealth"],
                stats["assault"],
                stats["tech"],
                stats["resilience"],
                abilities
            ])
    
    print(f"[OK] Exported Archetypes CSV to {csv_path}")


def export_weapons_csv(data: dict, output_dir: Path) -> None:
    """Export weapons to CSV for Unreal Data Table."""
    csv_path = output_dir / "Weapons.csv"
    
    with open(csv_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow([
            "Name",
            "Category",
            "Description",
            "DamageProfile"
        ])
        
        for weapon in data["weapons"]:
            writer.writerow([
                weapon["name"],
                weapon["category"],
                weapon["description"],
                weapon["damage_profile"]
            ])
    
    print(f"[OK] Exported Weapons CSV to {csv_path}")


def export_districts_csv(data: dict, output_dir: Path) -> None:
    """Export districts to CSV for Unreal Data Table."""
    csv_path = output_dir / "Districts.csv"
    
    with open(csv_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow([
            "Name",
            "Description",
            "Hazards",
            "EnemyProfiles"
        ])
        
        for district in data["districts"]:
            hazards = "|".join(district["hazards"])
            enemies = "|".join(district["enemy_profiles"])
            writer.writerow([
                district["name"],
                district["description"],
                hazards,
                enemies
            ])
    
    print(f"[OK] Exported Districts CSV to {csv_path}")


def export_factions_csv(data: dict, output_dir: Path) -> None:
    """Export factions to CSV for Unreal Data Table."""
    csv_path = output_dir / "Factions.csv"
    
    with open(csv_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow([
            "Name",
            "Philosophy",
            "SignatureTactics"
        ])
        
        for faction in data["factions"]:
            tactics = "|".join(faction["signature_tactics"])
            writer.writerow([
                faction["name"],
                faction["philosophy"],
                tactics
            ])
    
    print(f"[OK] Exported Factions CSV to {csv_path}")


def export_constants_cpp(data: dict, output_dir: Path) -> None:
    """Export constants to C++ header file."""
    cpp_path = output_dir / "GameConstants.h"
    
    complications = data["constants"]["complications"]
    extraction_conditions = data["constants"]["extraction_conditions"]
    
    # Import from data.py if available
    from neon_ascendant.data import COMPLICATIONS, EXTRACTION_CONDITIONS
    
    complications = list(COMPLICATIONS)
    extraction_conditions = list(EXTRACTION_CONDITIONS)
    
    cpp_content = f"""// Auto-generated constants from Python prototype
// DO NOT EDIT MANUALLY - Regenerate using scripts/export_to_unreal.py

#pragma once

#include "CoreMinimal.h"
#include "GameConstants.generated.h"

UCLASS()
class NEONASCENDANTCORE_API UGameConstants : public UObject
{{
    GENERATED_BODY()

public:
    // Mission Complications
    static const TArray<FString> GetComplications()
    {{
        return TArray<FString>{{
"""
    
    for comp in complications:
        cpp_content += f'            TEXT("{comp}"),\n'
    
    cpp_content += """        };
    }
    
    // Extraction Conditions
    static const TArray<FString> GetExtractionConditions()
    {
        return TArray<FString>{
"""
    
    for cond in extraction_conditions:
        cpp_content += f'            TEXT("{cond}"),\n'
    
    cpp_content += """        };
    }
    
    // Simulation Constants
    static constexpr int32 AbilitySynergyBonus = 2;
    static constexpr int32 ImplantSynergyBonus = 1;
    static constexpr int32 DiceRollMin = 1;
    static constexpr int32 DiceRollMax = 6;
    static constexpr int32 DifficultyVarianceMin = 0;
    static constexpr int32 DifficultyVarianceMax = 3;
};
"""
    
    with open(cpp_path, "w", encoding="utf-8") as f:
        f.write(cpp_content)
    
    print(f"[OK] Exported C++ constants to {cpp_path}")


def main() -> None:
    """Main export function."""
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    output_dir = project_root / "unreal_data"
    output_dir.mkdir(exist_ok=True)
    
    print("Exporting Neon Ascendant data to Unreal Engine formats...")
    print(f"Output directory: {output_dir}\n")
    
    # Export JSON (master format)
    data = export_codex_json(output_dir)
    
    # Export CSVs for Data Tables
    export_archetypes_csv(data, output_dir)
    export_weapons_csv(data, output_dir)
    export_districts_csv(data, output_dir)
    export_factions_csv(data, output_dir)
    
    # Export C++ constants
    export_constants_cpp(data, output_dir)
    
    print("\n[OK] Export complete!")
    print(f"\nNext steps:")
    print(f"1. Import CSV files into Unreal Data Tables")
    print(f"2. Copy GameConstants.h to your Unreal project")
    print(f"3. Use codex_export.json as reference for data asset creation")


if __name__ == "__main__":
    main()

