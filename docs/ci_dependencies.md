# CI/CD Dependencies

## DevkitPro Libraries

The GitHub Actions CI workflows automatically install the following development libraries using `dkp-pacman`:

### Core Packages
- **wii-dev**: Core development package for Nintendo Wii
- **libogc-wii**: Official Nintendo GameCube/Wii library
  - Provides hardware abstraction layer
  - Core system functionality

### Graphics & Media
- **ppc-grrlib**: Graphics library specifically for Wii homebrew
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
  - SD card and USB storage access
  - File I/O operations

## Installation Command

All libraries are installed in the CI environment using:

```bash
dkp-pacman -S wii-dev libogc-wii ppc-grrlib ppc-libpng ppc-zlib ppc-freetype ppc-libfat --noconfirm
```

## Local Development

For local development, ensure these same packages are installed in your DevkitPro environment:

```bash
# Update package database
dkp-pacman -Sy

# Install all required packages
dkp-pacman -S wii-dev libogc-wii ppc-grrlib ppc-libpng ppc-zlib ppc-freetype ppc-libfat
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
