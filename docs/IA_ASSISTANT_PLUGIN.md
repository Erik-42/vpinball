# IA Assistant Plugin

Plugin d'assistance IA pour Visual Pinball X - Permet l'analyse, la génération et l'amélioration de tables de flipper.

## Fonctionnalités

- **Analyse de tables existantes** : Analyse les éléments d'une table et suggère des améliorations
- **Génération d'éléments** : Génère des bumpers, targets, flippers, etc.
- **Suggestions de thèmes** : Crée des tables complètes à partir de thèmes spécifiés
- **Validation physique** : Vérifie les configurations physiques et propose des corrections

## Providers IA Supportés

| Provider | Description | Modèle par défaut |
|----------|-------------|-------------------|
| **freellmapi** | API gratuite Ollama | llama3.1:latest |
| **fcc** | Free Code Camp API | default |
| **local** | Ollama local | llama3.1:latest |
| **openai** | OpenAI GPT | gpt-4 |

## Installation

### Sur Linux

```bash
# Le plugin est inclus dans le package .deb
sudo dpkg -i vpinball_*.deb

# Vérifier l'installation
ls /usr/lib/vpinball/plugins/iaassistant/
```

### Configuration

1. Créez le fichier de configuration :
   ```bash
   mkdir -p ~/.config/vpinball/tables/iaassistant/
   cp /usr/share/doc/vpinball/examples/iaassistant.conf ~/.config/vpinball/iaassistant.conf
   ```

2. Configurez votre provider IA préféré :
   ```json
   {
       "api_provider": "freellmapi",
       "api_key": "votre_cle_api",
       "api_base_url": "https://api.freellmapi.com/v1",
       "temperature": 0.7,
       "max_tokens": 2048
   }
   ```

## Utilisation

### Interface Graphique

1. Lancez Visual Pinball X
2. Allez dans **Plugins > IA Assistant > Options**
3. Configurez votre provider IA
4. Utilisez les raccourcis clavier :
   - `Ctrl+I` : Analyser la table actuelle
   - `Ctrl+G` : Générer des éléments
   - `Ctrl+S` : Suggérer des améliorations

### Via Script Python

```python
from ia_assistant_example import IAAssistant, GenerationOptions

# Initialiser l'assistant
assistant = IAAssistant("~/.config/vpinball/iaassistant.conf")

# Analyser une table existante
result = assistant.analyze_table("/path/to/table.vpx")
improvements = result["response"]

# Générer une nouvelle table
options = GenerationOptions(
    manufacturer="Williams",
    theme="Space Adventure",
    difficulty="intermediate",
    include_scripts=True
)
new_table = assistant.generate_table(options)
```

### Via Command Line

```bash
# Analyser une table
vpinball -LiveEdit --ia-analyze table.vpx

# Générer une table
vpinball -IA-Generate --theme "Fantasy" --difficulty expert
```

## Format des Tables Générées

Le plugin génère des tables dans le format JSON suivant :

```json
{
    "table_name": "Space Invaders",
    "manufacturer": "Williams",
    "elements": [
        {
            "type": "bumper",
            "name": "Bumper1",
            "x": 320,
            "y": 240,
            "z": 0,
            "rotation": 0,
            "properties": {
                "size": "medium",
                "score": 100
            }
        },
        {
            "type": "flipper",
            "name": "LeftFlipper",
            "x": 100,
            "y": 350,
            "angle": "180"
        }
    ],
    "physics": {
        "gravity": 9.81,
        "mass": 1.0,
        "friction": 0.5
    },
    "scripts": {
        "vbs": "Sub Main()\nEnd Sub"
    }
}
```

## Configuration par Provider

### FreeLlmApi

```json
{
    "api_provider": "freellmapi",
    "api_key": "your_api_key",
    "api_base_url": "https://api.freellmapi.com/v1",
    "model": "llama3.1:latest"
}
```

### FCC (Free Code Camp)

```json
{
    "api_provider": "fcc",
    "api_key": "your_api_key",
    "api_base_url": "https://api.fcc.dev"
}
```

### Ollama Local

```bash
# Installer Ollama
curl -fsSL https://ollama.com/install.sh | sh

# Télécharger le modèle
ollama pull llama3.1:latest

# Configurer le plugin
{
    "api_provider": "local",
    "api_base_url": "http://localhost:11434",
    "model": "llama3.1:latest"
}
```

## Développement

### Structure du Plugin

```
plugins/iaassistant/
├── ia_assistant.cpp      # Point d'entrée principal
├── ai_client.cpp         # Clients API IA
├── table_analyzer.cpp    # Analyse de tables
├── ia_assistant.h        # En-têtes
├── common.h              # Définitions communes
└── plugin.cfg            # Configuration du plugin
```

### Compiler le Plugin

```bash
# Depuis le répertoire racine du projet
cmake -DBUILD_APP=ON -DRENDERER=BGFX -DPLATFORM=linux -DARCH=x64 -B build
cmake --build build --target IAAssistantPlugin
```

## API du Plugin

### Fonctions Exportées

| Fonction | Description |
|----------|-------------|
| `InitializePlugin(hwnd_parent)` | Initialise le plugin |
| `ProcessPlugin()` | Traitement chaque frame |
| `ShutdownPlugin()` | Nettoyage |
| `ShowOptionsDialog(hwnd_parent)` | Affiche les options |
| `AnalyzeCurrentTable()` | Analyse la table active |
| `GenerateTable(theme, difficulty)` | Génère une nouvelle table |

## Licence

GPL-3.0-or-later - Voir le fichier LICENSE du projet principal.

## Contributing

Les contributions sont les bienvenues ! Veuillez soumettre vos modifications via des Pull Requests.