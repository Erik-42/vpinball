#!/usr/bin/env bash
# Installation de VPinball pour Linux
# Ce script installe le binaire, les fichiers .desktop et l'icône

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BIN_NAME="VPinballX_BGFX"
INSTALL_PREFIX="${INSTALL_PREFIX:-/usr/local}"
DESKTOP_DIR="${HOME}/.local/share/applications"
ICON_DIR="${HOME}/.local/share/icons/hicolor/256x256/apps"
BIN_DIR="${INSTALL_PREFIX}/bin"

echo "📦 Installation de VPinball..."

# 1. Vérifier que le binaire existe
if [[ ! -f "${SCRIPT_DIR}/${BIN_NAME}" ]]; then
    echo "❌ Erreur: ${BIN_NAME} introuvable dans ${SCRIPT_DIR}"
    exit 1
fi

# 2. Installer le binaire
echo "🔧 Installation du binaire dans ${BIN_DIR}..."
sudo install -Dm755 "${SCRIPT_DIR}/${BIN_NAME}" "${BIN_DIR}/vpinball"

# 3. Créer les répertoires si nécessaire
mkdir -p "${DESKTOP_DIR}" "${ICON_DIR}"

# 4. Installer l'icône
echo "🎨 Installation de l'icône..."
cp "${SCRIPT_DIR}/src/assets/vpinball.png" "${ICON_DIR}/vpinball.png"

# 5. Installer les fichiers .desktop
echo "📋 Installation des raccourcis..."
cp "${SCRIPT_DIR}/vpinball-play.desktop" "${DESKTOP_DIR}/vpinball-play.desktop"
cp "${SCRIPT_DIR}/vpinball-editor.desktop" "${DESKTOP_DIR}/vpinball-editor.desktop"

# 6. Rendre exécutables
chmod +x "${DESKTOP_DIR}/vpinball-play.desktop"
chmod +x "${DESKTOP_DIR}/vpinball-editor.desktop"

# 7. Mettre à jour la base de données des applications
if command -v update-desktop-database &> /dev/null; then
    update-desktop-database "${DESKTOP_DIR}" 2>/dev/null || true
fi

# 8. Mettre à jour le cache des icônes
if command -v gtk-update-icon-cache &> /dev/null; then
    gtk-update-icon-cache -f -t "${HOME}/.local/share/icons/hicolor" 2>/dev/null || true
fi

echo "✅ Installation terminée !"
echo ""
echo "Vous pouvez maintenant lancer :"
echo "  • 'VPinball – Jouer' depuis le menu des applications"
echo "  • 'VPinball – Éditeur' depuis le menu des applications"
echo "  • Ou en ligne de commande : vpinball -play"
echo ""
echo "Pour lancer une table spécifique :"
echo "  vpinball -play /chemin/vers/table.vpz"