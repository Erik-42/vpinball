#!/bin/bash
# Script de vérification de sécurité
# Recherche les éventuels tokens ou secrets exposés dans le codebase

echo "🛡️  Vérification de sécurité du projet vpinball..."
echo "=================================================="
echo ""

# Fichiers à analyser (exclure node_modules, build, etc.)
EXCLUDE_ARGS="--exclude-dir=node_modules --exclude-dir=build --exclude-dir=.git"

ISSUES_FOUND=0

echo "1️⃣ Recherche des tokens API Freellmapi..."
if git grep -i "freellmapi" $EXCLUDE_ARGS 2>/dev/null; then
    ISSUES_FOUND=$((ISSUES_FOUND + 1))
    echo "⚠️  Tokens freellmapi trouvés!"
else
    echo "✅ Aucun token freellmapi trouvé"
fi

echo ""
echo "2️⃣ Recherche d'autres patterns sensibles..."

# Rechercher d'autres patterns de tokens/clés
SENSITIVE_PATTERNS=(
    "api_key"
    "apikey"
    "secret"
    "password"
    "token"
    "credential"
    "auth_token"
    "private_key"
)

for pattern in "${SENSITIVE_PATTERNS[@]}"; do
    if git grep -i "$pattern" $EXCLUDE_ARGS -- "*.json" "CMakeLists.txt" "*.cpp" "*.h" "*.py" "*.sh" 2>/dev/null | grep -v ".env.example" | grep -v "docs/SECURITY.md" | grep -v "README.md"; then
        echo "⚠️  Pattern '$pattern' trouvé"
        ISSUES_FOUND=$((ISSUES_FOUND + 1))
    fi
done

echo ""
echo "3️⃣ Vérification des fichiers non suivis..."
UNTRACKED_FILES=$(git status --porcelain | grep "^??" | wc -l)
if [ "$UNTRACKED_FILES" -gt 0 ]; then
    echo "⚠️  $UNTRACKED_FILES fichiers non suivis"
    echo "   Liste:"
    git status --porcelain | grep "^??" | head -10
fi

echo ""
echo "4️⃣ Vérification des bibliothèques tierces..."
THIRD_PARTY_LIBS=$(ls -la lib*.so* 2>/dev/null | wc -l || echo "0")
if [ "$THIRD_PARTY_LIBS" -gt 0 ]; then
    echo "⚠️  Bibliothèques tierces dans la racine ($THIRD_PARTY_LIBS fichiers)"
fi

echo ""
echo "=================================================="
if [ "$ISSUES_FOUND" -eq 0 ]; then
    echo "✅ Aucune faille de sécurité détectée!"
    exit 0
else
    echo "⚠️  $ISSUES_FOUND problème(s) de sécurité trouvé(s)"
    exit 1
fi