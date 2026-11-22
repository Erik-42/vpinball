# Debian Packaging for Visual Pinball X

This document describes the Debian packaging for Visual Pinball X.

## Package Structure

The packaging consists of three packages:

1. **vpinball** - Main binary package
   - Application executable
   - Shared libraries (SDL3, bgfx, etc.)
   - Plugin modules
   - Assets and documentation

2. **vpinball-common** - Architecture-independent files
   - Documentation files
   - Desktop integration files (.desktop)
   - Application icons
   - README and LICENSE

3. **vpinball-dbg** - Debug symbols
   - Separate package for debugging symbols
   - Depends on exact version of vpinball

## Building the Package

### Prerequisites

Install build dependencies:

```bash
sudo apt-get install build-essential cmake pkg-config
sudo apt-get install debhelper dh-make
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev
sudo apt-get install libx11-dev libxext-dev libxrandr-dev
sudo apt-get install libopenal-dev libvorbis-dev libogg-dev
sudo apt-get install libsdl3-dev libsdl3-ttf-dev libfreeimage-dev
sudo apt-get install libhidapi-libusb-dev libftdi1-dev libserialport-dev
sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev
sudo apt-get install libmysofa-dev libopus-dev libudev-dev
sudo apt-get install libdrm-dev libgbm-dev libwayland-dev
sudo apt-get install libegl-dev mesa-vulkan-drivers
```

### Build Steps

```bash
# 1. Create build directory
mkdir -p build
cd build

# 2. Configure CMake
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_APP=ON \
  -DRENDERER=BGFX \
  -DPLATFORM=linux \
  -DARCH=x64 \
  -DENABLE_XR=ON

# 3. Build
make -j$(nproc)

# 4. Build Debian package
cd ..
dpkg-buildpackage -us -uc -b
```

## Installation

```bash
sudo dpkg -i ../vpinball_*.deb
```

## Dependencies

The package depends on:
- libgl1, libglx0 - OpenGL
- libsdl3-0, libsdl3-ttf-0 - SDL3 libraries
- libfreeimage3 - Image loading
- libopenal1 - Audio
- libx11-6, libxext6, libxrandr2 - X11
- libwayland-client0 - Wayland support
- libegl1 - EGL for rendering

## Configuration

- User data: `~/.config/vpinball/`
- Tables: `~/.config/vpinball/tables/`
- Plugins: `/usr/lib/vpinball/plugins/`

## VR/XR Support

For VR support, additional packages may be required:
- `openxr-demo` or SteamVR runtime
- VR headset drivers

## Troubleshooting

### Library not found errors

If the application fails to start with library errors:

```bash
# Check for missing libraries
ldd /usr/bin/vpinball

# Update library cache
sudo ldconfig
```

### Desktop integration

```bash
# Update desktop database
update-desktop-database

# Update icon cache
gtk-update-icon-cache
```

## Version Scheme

Debian package version format: `<upstream_version>-<debian_revision>`

Example: `10.8.1-1`

## Changelog

See `debian/changelog` for version history.

## License

GPL-3.0-or-later - See `debian/copyright` for details.