# Visual Pinball

*An open source pinball table editor and simulator.*

This project was started by Randy Davis, open sourced in 2010 and continued by the Visual Pinball development team. This is the official repository.

## Features

- Simulates pinball table physics and renders the table with DirectX, OpenGL or [bgfx](https://bkaradzic.github.io/bgfx/overview.html)
- Simple editor to (re-)create any kind of pinball table
- Live editing of most content within the rendered viewport
- Table logic (and game rules) can be controlled via Visual Basic Script
- Over 1050 real/unique pinball machines from ~100 manufacturers, plus over 550 original creations were rebuilt/designed using the Visual Pinball X editor (over 3000 if one counts all released tables, incl. MODs and different variants), and even more when including its predecessor versions (Visual Pinball 9.X)
- Emulation of real pinball machines via [PinMAME](https://github.com/vpinball/pinmame) is possible via Visual Basic Script (Visual PinMAME), or via the libPinMAME-API/plugin
- Supports configurable camera views (e.g. to allow for correct display in virtual pinball cabinets)
- Support for Tablet/Touch input, Joypads, or specialized pinball controllers
- Support for Stereo3D output
- Support for Head tracking via BAM
- Support for VR/XR HMD rendering (including [PUP](https://www.nailbuster.com/wikipinup), [B2S](https://github.com/vpinball/b2s-backglass) backglass and DMD output support)
- Support for WCG/HDR rendering (for now only via the BGFX (D3D11/12) build)
- Support for Windows (x86), Linux (x86/Arm, incl. RaspberryPi and RK3588), macOS, iOS/iPadOS, Android and Meta Quest (the iOS and Android builds are also available via the respective app stores for free)
- Plugin system to drive/fuel all kinds of displays (DMD, backglass, etc), add custom/dynamically-changed content (PUP, Serum, etc), direct output framework (DOF), sensors, and much more (WIP)

## Download

All releases are available on the [releases page](https://github.com/vpinball/vpinball/releases).

## Documentation

Documentation is currently sparse. Check the [docs](docs) directory for various guides and references.

An [unofficial wiki](https://github.com/dekay/vpinball-wiki) is currently being developed. Community contributions, suggestions, and help are welcome to improve the resource for all users.

## How to build

Build instructions are available in the [make directory README](make/README.md).

## Installation sur Linux

### Via le paquet .deb (recommandé)

Téléchargez le dernier paquet `.deb` depuis la [page des releases](https://github.com/vpinball/vpinball/releases) et installez-le :

```bash
sudo dpkg -i vpinball_<version>_amd64.deb
sudo apt-get install -f  # au cas où des dépendances manqueraient
```

Cela installe :
- Le binaire `vpinball` dans `/usr/local/bin/`
- Les entrées de menu **VPinball – Jouer** et **VPinball – Éditeur**
- L'icône de l'application

### Via le script d'installation manuelle

Si vous avez téléchargé l'archive `vpinball-linux.tar.gz` depuis les releases :

```bash
tar xzf vpinball-linux.tar.gz
cd vpinball-linux
sudo ./install.sh
```

Le script installe le binaire, les raccourcis `.desktop` et l'icône dans `~/.local/share/`.

### Lancer une table spécifique

```bash
# Jouer à une table
vpinball -play /chemin/vers/table.vpz

# Ouvrir l'éditeur
vpinball -LiveEdit /chemin/vers/table.vpz
```

Si aucune table n'est spécifiée avec `-play`, le jeu démarre et vous permet de choisir une table.

## Participate

Read through our [contribution](CONTRIBUTING.md) rules carefully before opening PRs, please.
