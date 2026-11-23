#!/usr/bin/env python3
"""
IA Assistant Plugin - Script d'exemple
Démonstration de l'analyse et génération de tables de flipper
"""

import json
import os
import sys
from typing import Dict, List, Optional, Any
from dataclasses import dataclass, field, asdict
from enum import Enum

class ElementType(Enum):
    BUMPER = "bumper"
    FLIPPER = "flipper"
    TARGET = "target"
    DRAIN = "drain"
    SILLOSCOPE = "silloscope"
    LIGHT = "light"
    PRALL = "prall"
    TRIGGER = "trigger"
    SILL = "sill"

@dataclass
class TableElement:
    type: ElementType
    name: str
    x: float
    y: float
    z: float = 0.0
    rotation: float = 0.0
    properties: Dict[str, Any] = field(default_factory=dict)

@dataclass
class GenerationOptions:
    manufacturer: str = "Custom"
    theme: str = "Generic"
    difficulty: str = "intermediate"
    include_scripts: bool = True
    include_music: bool = False

class IAAssistant:
    """Assistant IA pour la création de tables de flipper"""

    def __init__(self, config_path: str = None):
        self.config = self._load_config(config_path)
        self.api_provider = self.config.get("api_provider", "freellmapi")
        self.api_key = self.config.get("api_key", "")
        self.api_base_url = self.config.get("api_base_url", "")

    def _load_config(self, config_path: str) -> Dict:
        """Charger la configuration depuis un fichier"""
        if config_path and os.path.exists(config_path):
            with open(config_path, 'r') as f:
                return json.load(f)
        return {}

    def analyze_table(self, table_path: str) -> Dict[str, Any]:
        """Analyser une table existante et suggérer des améliorations"""
        if not os.path.exists(table_path):
            return {"error": f"Table not found: {table_path}"}

        # Analyse simplifiée - dans la version complète, parser le .vpx
        elements = self._extract_elements(table_path)

        prompt = f"""
Analyze this Visual Pinball X table and suggest improvements:

Table: {table_path}
Elements found: {len(elements)}

{self._format_elements(elements)}

Provide suggestions in JSON format with 'improvements' array containing:
- type: element type (bumper, flipper, target, etc.)
- suggestion: description of improvement
- priority: high, medium, low
        """

        return self._call_api(prompt)

    def _extract_elements(self, table_path: str) -> List[TableElement]:
        """Extraire les éléments d'une table (analyse simplifiée)"""
        # Version stub - parserait le format VPX dans l'implémentation complète
        # Pour l'exemple, retourner des éléments génériques
        return [
            TableElement(ElementType.BUMPER, "Bumper1", 320, 240),
            TableElement(ElementType.FLIPPER, "LeftFlipper", 100, 350),
            TableElement(ElementType.FLIPPER, "RightFlipper", 540, 350),
            TableElement(ElementType.DRAIN, "Drain", 320, 400),
            TableElement(ElementType.TARGET, "Target1", 200, 300),
            TableElement(ElementType.TARGET, "Target2", 440, 300),
        ]

    def _format_elements(self, elements: List[TableElement]) -> str:
        """Formater les éléments pour le prompt"""
        lines = []
        for elem in elements:
            lines.append(f"- {elem.type.value}: {elem.name} at ({elem.x}, {elem.y})")
        return "\n".join(lines)

    def generate_table(self, options: GenerationOptions) -> Dict[str, Any]:
        """Générer une nouvelle table avec les spécifications données"""
        prompt = f"""
Generate a complete Visual Pinball X table with these specifications:

Manufacturer: {options.manufacturer}
Theme: {options.theme}
Difficulty: {options.difficulty}
Include scripts: {options.include_scripts}
Include music: {options.include_music}

Return a JSON structure with:
- table_name: string
- elements: array of elements (bumper, flipper, target, drain, silloscope, light, trigger, prall)
- physics: physics settings
- scripts: optional VBScript code

For each element include: name, type, x, y coordinates (in pixels), and properties.
        """

        return self._call_api(prompt)

    def suggest_improvements(self, table_path: str) -> Dict[str, Any]:
        """Suggérer des améliorations pour une table existante"""
        elements = self._extract_elements(table_path)

        prompt = f"""
Suggest specific improvements for this pinball table:

Elements: {len(elements)}

{self._format_elements(elements)}

Return JSON with 'suggestions' array, each containing:
- element: element name
- suggestion: improvement suggestion
- priority: high, medium, low
- estimated_effort: low, medium, high
        """

        return self._call_api(prompt)

    def _call_api(self, prompt: str) -> Dict[str, Any]:
        """Appeler l'API IA (stub - implémentation complète)"""
        # Cette fonction ferait un appel HTTP réel à l'API
        # Pour l'exemple, retourner une réponse statique
        return {
            "success": True,
            "response": json.dumps({
                "improvements": [
                    {"type": "bumper", "suggestion": "Add collision sound", "priority": "medium"},
                    {"type": "light", "suggestion": "Add light sequence for scoring", "priority": "high"}
                ]
            }, indent=2)
        }

def main():
    """Exemple d'utilisation"""
    # Charger la configuration
    config_path = os.path.expanduser("~/.config/vpinball/iaassistant.conf")
    assistant = IAAssistant(config_path)

    # Exemple 1: Analyser une table existante
    if len(sys.argv) > 1:
        table_path = sys.argv[1]
        print(f"Analyzing table: {table_path}")
        result = assistant.analyze_table(table_path)
        print(json.dumps(result, indent=2))

    # Exemple 2: Générer une nouvelle table
    else:
        options = GenerationOptions(
            manufacturer="Williams",
            theme="Space",
            difficulty="intermediate",
            include_scripts=True
        )
        print("Generating new table...")
        result = assistant.generate_table(options)
        print(json.dumps(result, indent=2))

if __name__ == "__main__":
    main()