# CI/CD Dependencies

## DevkitPro Libraries

The GitHub Actions CI workflows automatically install the following development libraries using `dkp-pacman`:

### Core Packages
- **libogc**: Official Nintendo GameCube/Wii library
  - Provides hardware abstraction layer
  - Core system functionality
- **libfat-ogc**: File system access library for GameCube/Wii
  - SD card and USB storage access
  - File I/O operations
- **devkitppc-rules**: Build rules for PowerPC cross-compilation

### Graphics & Media
- **ppc-grrlib**: Graphics library specifically for Wii homebrew
  - **Note**: Compiled from source (https://github.com/GRRLIB/GRRLIB)
  - Not available in official DevkitPro repositories
  - 2D/3D graphics rendering
  - Sprite and texture support
  - Color manipulation functions
- **ppc-libpng**: PNG image format support
  - Required for loading PNG textures
- **ppc-zlib**: Compression library
  - Required by libpng for compressed data
- **ppc-freetype**: Font rendering library
  - Text rendering capabilities

### System Libraries
- **ppc-libfat**: File system access library
  - Note: Use `libfat-ogc` package name
  - SD card and USB storage access
  - File I/O operations

## Installation Command

Core libraries are installed in the CI environment using:

```bash
dkp-pacman -S libogc libfat-ogc devkitppc-rules ppc-libpng ppc-zlib ppc-freetype --noconfirm
```

GRRLIB is compiled from source:

```bash
cd /tmp
git clone https://github.com/GRRLIB/GRRLIB.git
cd GRRLIB/GRRLIB
make clean all install
```

## Local Development

For local development, install the core packages and compile GRRLIB:

```bash
# Update package database
dkp-pacman -Sy

# Install core packages
dkp-pacman -S libogc libfat-ogc devkitppc-rules ppc-libpng ppc-zlib ppc-freetype

# Compile and install GRRLIB
cd /tmp
git clone https://github.com/GRRLIB/GRRLIB.git
cd GRRLIB/GRRLIB
make clean all install
```

## Troubleshooting

### Missing Headers
If you encounter errors like "grrlib.h: No such file or directory":
1. Verify all packages are installed
2. Check DEVKITPRO and DEVKITPPC environment variables
3. Ensure library paths are in the linker configuration

### Compilation Errors
- Verify PowerPC cross-compiler is available: `powerpc-eabi-gcc --version`
- Check Makefile includes proper library flags
- Ensure all dependencies are installed before building
