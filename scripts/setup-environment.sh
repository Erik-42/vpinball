#!/bin/bash
# Script d'initialisation sécurisée du projet vpinball
# Usage: ./scripts/setup-environment.sh

set -e

echo "🔧 Configuration de sécurité pour vpinball..."

# Vérifier si .env existe
if [ ! -f .env ]; then
    if [ -f .env.example ]; then
        echo "📝 Copie du fichier .env.example vers .env"
        cp .env.example .env
        echo "⚠️  Veuillez éditer .env et ajouter votre clé API"
    else
        echo "❌ Fichier .env.example non trouvé"
        exit 1
    fi
else
    echo "✅ Fichier .env déjà existe"
fi

# Vérifier si direnv est disponible
if command -v direnv &> /dev/null; then
    echo ""
    echo "🎯 direnv détecté - configuration recommandée:"
    echo "   1. Ajoutez vos variables dans .env"
    echo "   2. Exécutez: echo 'source .env' > .envrc"
    echo "   3. Exécutez: direnv allow"
else
    echo ""
    echo "💡 sans direnv, exportez manuellement les variables:"
    echo "   source .env"
fi

# Fonction pour charger les variables
load_env() {
    if [ -f .env ]; then
        set -a
        source .env
        set +a
        echo "✅ Variables d'environnement chargées depuis .env"
    fi
}

# Vérifier les tokens exposés dans le git
echo ""
echo "🔎 Vérification des tokens potentiellement exposés..."
if git grep -q "freellmapi" 2>/dev/null; then
    echo "⚠️  Attention: Un token freellmapi a été trouvé dans l'historique git"
    echo "   Commande pour vérifier: git log -p -S 'freellmapi'"
else
    echo "✅ Aucun token freellmapi exposé trouvé"
fi

# Vérifier les fichiers sensibles dans .gitignore
echo ""
echo "📋 Vérification du .gitignore..."
if grep -q "freellmapi-" .gitignore 2>/dev/null; then
    echo "✅ .gitignore contient les règles de sécurité appropriées"
else
    echo "⚠️  .gitignore ne contient pas les règles de sécurité"
fi

echo ""
echo "✅ Configuration terminée avec succès!"
echo ""
echo "📝 Prochaines étapes:"
echo "   1. Éditez .env et ajoutez votre ANTHROPIC_AUTH_TOKEN"
echo "   2. Re-vérifiez l'historique git pour les tokens exposés"
echo "   3. Utilisez 'source .env' avant de lancer Claude Code"