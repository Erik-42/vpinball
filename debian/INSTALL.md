# Installation de Visual Pinball X sous Debian/Ubuntu

## Méthode 1: Installation depuis un package Debian (.deb)

### Téléchargement

```bash
# Télécharger les fichiers .deb depuis GitHub Releases
wget https://github.com/vpinball/vpinball/releases/latest/download/vpinball_*.deb
```

### Installation

```bash
# Installer le package principal
sudo dpkg -i vpinball_*.deb

# Si des dépendances manquent, les corriger
sudo apt-get install -f

# Vérifier l'installation
dpkg -l | grep vpinball
```

### Désinstallation

```bash
# Supprimer le package
sudo dpkg -r vpinball

# Supprimer également les fichiers communs
sudo dpkg -r vpinball-common

# Supprimer les symboles de debug si installés
sudo dpkg -r vpinball-dbg

# Nettrer les fichiers de configuration
sudo dpkg -P vpinball vpinball-common
```

## Méthode 2: Construction depuis les sources

### Prérequis

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake pkg-config
sudo apt-get install -y debhelper dh-make
sudo apt-get install -y libgl1-mesa-dev libglu1-mesa-dev
sudo apt-get install -y libx11-dev libxext-dev libxrandr-dev
sudo apt-get install -y libopenal-dev libvorbis-dev libogg-dev
sudo apt-get install -y libsdl3-dev libsdl3-ttf-dev libfreeimage-dev
sudo apt-get install -y libhidapi-libusb-dev libftdi1-dev libserialport-dev
sudo apt-get install -y libavcodec-dev libavformat-dev libavutil-dev
sudo apt-get install -y libmysofa-dev libopus-dev libudev-dev
sudo apt-get install -y libdrm-dev libgbm-dev libwayland-dev
sudo apt-get install -y libegl-dev mesa-vulkan-drivers
```

### Construction du package

```bash
# 1. Cloner le dépôt
git clone https://github.com/vpinball/vpinball.git
cd vpinball

# 2. Construire le package Debian
dpkg-buildpackage -us -uc -b

# 3. Les fichiers .deb seront créés dans le répertoire parent
ls -la ../vpinball_*.deb
```

## Vérification de l'installation

### Vérifier les dépendances

```bash
ldd /usr/bin/vpinball
```

### Vérifier les fichiers installés

```bash
dpkg -L vpinball | head -20
ls -la ~/.config/vpinball/tables/
```

### Vérifier les icônes

```bash
gtk-update-icon-cache /usr/share/icons/hicolor
update-desktop-database
```

## Configuration

### Répertoires importants

| Emplacement | Description |
|-------------|-------------|
| `~/.config/vpinball/` | Répertoire de configuration utilisateur |
| `~/.config/vpinball/tables/` | Tables de pinball installées |
| `/usr/lib/vpinball/plugins/` | Modules/plugins installés |
| `/usr/share/vpinball/` | Données partagées |

### Variables d'environnement

```bash
# Ajouter au PATH (si nécessaire)
export PATH=$PATH:/usr/lib/vpinball

# Variables pour les tables
export VPINBALL_TABLES=~/.config/vpinball/tables
```

## Résolution des problèmes

### Erreur: impossible de trouver libSDL3.so

```bash
# Vérifier les bibliothèques
ldconfig -p | grep SDL3

# Mettre à jour le cache
sudo ldconfig
```

### Erreur: écran noir au démarrage

```bash
# Vérifier les permissions OpenGL
glxinfo | grep -i "direct rendering"

# Vérifier les pilotes vidéo
glxinfo | grep -i "OpenGL renderer"
```

### Problèmes avec OpenXR/VR

```bash
# Vérifier OpenXR
openxr-cli enum_instance_extensions

# Vérifier les casques VR détectés
xrcli discover
```

## Support

- GitHub: https://github.com/vpinball/vpinball
- Documentation: https://vpinball.com/
- Issues: https://github.com/vpinball/vpinball/issues