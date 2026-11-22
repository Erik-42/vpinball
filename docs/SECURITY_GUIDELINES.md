# Lignes directrices de sécurité

## Résumé

Ce document décrit les bonnes pratiques de sécurité pour le développement et la contribution au projet Visual Pinball.

## Configuration des secrets

### Variables d'environnement

**Toujours utiliser des variables d'environnement pour les secrets**, jamais de valeurs hardcodées dans le code.

#### Configuration recommandée:

1. **Utiliser `.env` local** (non versionné)
2. **Documenter avec `.env.example`** (versionné)
3. **Charger automatiquement les variables**

```bash
# Au démarrage de développement
source .env

# Ou avec direnv
echo 'source .env' > .envrc
direnv allow
```

### Types de secrets à protéger

- 🔑 Tokens API (Anthropic, Google, etc.)
- 🔐 Clés API (OpenAI, Azure, etc.)
- 🗝️ Secrets de déploiement
- 📧 Clés de compte service
- 🔐 Certificats SSL/TLS
- 🧾 Identifiants de base de données

## Sécurité du build

### Bibliothèques tierces

Les bibliothèques tierces doivent être:
- ✅ Building via les scripts `external.sh`
- ✅ Non versionnées dans le dépôt
- ✅ Vérifiées pour les vulnérabilités

### Fichiers non suivis

Les fichiers suivants ne doivent **jamais** être commités:

```
.env              # Variables d'environnement locales
*.key             # Clés privées
*.pem             # Certificats
 credentials.json # Identifiants
 secret/           # Dossiers secrets
```

## Revue de code

### Avant chaque merge

1. **Scanner les secrets** avec les scripts de sécurité
2. **Revérifier les fichiers `.env`** ne sont pas commités
3. **Vérifier les fichiers de configuration** sensibles

### Outils recommandés

```bash
# Scanner les secrets dans le git history
git secrets --scan-history

# Ou avec truffleHog
trufflehog git file:///path/to/repo

# Vérification rapide
./scripts/security-check.sh
```

## Sécurité des plugins

Les plugins tiers doivent être:
- ✅ Revus pour sécurité
- ✅ Téléchargés depuis des sources fiables
- ✅ Vérifiés avec checksums

## Signalement des vulnérabilités

Si vous découvrez une vulnérabilité de sécurité:

1. **Ne pas créer d'issue publique**
2. Contacter les mainteneurs privéement via GitHub Security Advisories
3. Attendre le correctif avant tout divulgation publique

## Formation et sensibilisation

### Pour les contributeurs

- Lisez ces lignes directrices avant de commencer
- Utilisez toujours `.env.example` comme template
- Faites un `git diff` avant chaque commit pour vérifier les secrets

### Pour les mainteneurs

- Exécutez `scripts/security-check.sh` dans les CI
- Utilisez `gitleaks` ou `trufflehog` dans les workflows
- Formez les nouveaux contributeurs à la sécurité

## Historique des incidents

### Incidents corrigés

1. **Token API exposé** (2024)
   - Token `freellmapi-xxx` dans configuration
   - **Solution**: Déplacer vers variables d'environnement
   - **Prévention**: Ajouter au .gitignore et scanner CI

2. **Fichiers binaires dans git** (2024)
   - Bibliothèques `.so` dans la racine
   - **Solution**: Utiliser les scripts de build externes
   - **Prévention**: Mettre à jour .gitignore

## Références

- [GitHub Security Guide](https://docs.github.com/fr/code-security)
- [OWASP Top 10](https://owasp.org/www-project-top-ten/)
- [Git Secrets](https://github.com/awslabs/git-secrets)
- [TruffleHog](https://github.com/trufflesecurity/trufflehog)