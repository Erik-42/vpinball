# Guide de Sécurité du Projet

## Configuration des tokens API

### Problème
Le fichier `~/.claude/settings.json` contient potentiellement des tokens API hardcodés qui ne doivent pas être versionnés.

### Solution recommandée

#### Étape 1: Utiliser les variables d'environnement

Créez un fichier `.env` à la racine du projet:

```bash
# Copier le template
cp .env.example .env

# Éditer et remplir vos valeurs
nano .env
```

#### Étape 2: Configurer Claude Code pour lire depuis .env

Dans votre fichier `~/.claude/settings.json`, remplacez:

```json
{
  "env": {
    "ANTHROPIC_AUTH_TOKEN": "freellmapi-xxxxxxxxx"
  }
}
```

Par:

```json
{
  "env": {
    "ANTHROPIC_BASE_URL": "http://localhost:3001",
    "ANTHROPIC_MODEL": "auto",
    "ANTHROPIC_DEFAULT_OPUS_MODEL": "auto",
    "ANTHROPIC_DEFAULT_SONNET_MODEL": "auto",
    "ANTHROPIC_DEFAULT_HAIKU_MODEL": "auto",
    "CLAUDE_CODE_AUTO_COMPACT_WINDOW": "131072",
    "CLAUDE_CODE_ENABLE_GATEWAY_MODEL_DISCOVERY": "1"
  }
}
```

#### Étape 3: Charger les variables d'environnement

Lors de l'utilisation de Claude Code:

```bash
# Option 1: Export temporaire
export ANTHROPIC_AUTH_TOKEN="freellmapi-xxxxxxxxx"
claude

# Option 2: Avec direnv (recommandé)
echo 'export ANTHROPIC_AUTH_TOKEN="freellmapi-xxxxxxxxx"' > .envrc
direnv allow
```

### Bonnes pratiques de sécurité

1. **Toujours utiliser .env pour les secrets**
2. **Ne jamais committer .env**
3. **Utiliser .env.example comme référence**
4. **Vérifier les fichiers sensibles dans le git**

### Vérification

Pour vérifier qu'aucun token n'est exposé dans le git:

```bash
# Chercher les tokens exposés
git grep -i "freellmapi"

# Chercher d'autres patterns sensibles
git grep -i "api_key\|password\|token\|secret" -- "*.json"
```

---

## Fichiers non suivis Git

Plusieurs fichiers de bibliothèques tierces figurent parmi les fichiers non suivis Git. Ces fichiers seront automatiquement ignorés lorsque les dépendances seront compilées via le script `external.sh`.