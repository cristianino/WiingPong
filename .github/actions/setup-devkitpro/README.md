# Setup DevkitPro Action

A robust GitHub Action for installing DevkitPro toolchain with multiple fallback methods.

## Features

- **Multiple Installation Methods**: APT repository (preferred) with fallback to direct downloads
- **Robust Error Handling**: Tries multiple sources if primary method fails  
- **Caching Support**: Caches DevkitPro installation for faster subsequent builds
- **Automatic Verification**: Ensures tools are properly installed and accessible
- **Flexible Package Selection**: Install any DevkitPro packages (wii-dev, 3ds-dev, etc.)

## Usage

```yaml
- name: Setup DevkitPro
  uses: ./.github/actions/setup-devkitpro
  with:
    packages: 'wii-dev'  # Optional: space-separated list of packages
```

## Inputs

| Input | Description | Required | Default |
|-------|-------------|----------|---------|
| `packages` | DevkitPro packages to install (space-separated) | No | `wii-dev` |

## Outputs

| Output | Description |
|--------|-------------|
| `devkitpro-version` | Installed DevkitPro version string |

## Installation Methods

The action tries multiple methods in order:

### 1. APT Repository (Preferred)
- Adds official DevkitPro APT repository
- Installs via `apt install devkitpro-pacman`
- Most reliable and up-to-date method

### 2. Direct Download (Fallback)
- Downloads `.deb` package from GitHub releases
- Tries multiple versions for compatibility
- Fallback when APT repository is unavailable

## Environment Variables

The action automatically sets:
- `DEVKITPRO=/opt/devkitpro`
- `DEVKITPPC=/opt/devkitpro/devkitPPC`
- `DEVKITARM=/opt/devkitpro/devkitARM`

And adds to `PATH`:
- `/opt/devkitpro/tools/bin`
- `/opt/devkitpro/pacman/bin`

## Caching

The action caches the DevkitPro installation to speed up subsequent builds:
- **Cache Key**: `devkitpro-{os}-{action-hash}`
- **Cache Path**: `/opt/devkitpro`
- **Invalidation**: When action.yml is modified

## Available Packages

Common DevkitPro packages you can install:

- `wii-dev` - Wii homebrew development
- `gamecube-dev` - GameCube homebrew development  
- `3ds-dev` - Nintendo 3DS development
- `switch-dev` - Nintendo Switch development
- `nds-dev` - Nintendo DS development
- `gba-dev` - Game Boy Advance development

## Example: Multiple Packages

```yaml
- name: Setup DevkitPro for multiple consoles
  uses: ./.github/actions/setup-devkitpro
  with:
    packages: 'wii-dev gamecube-dev'
```

## Troubleshooting

### Installation Failures
The action includes comprehensive error handling and will try multiple methods. Check the logs for:
1. APT repository accessibility
2. Package download success
3. Installation verification

### Cache Issues
If builds are failing due to cached state:
1. Clear the cache in repository settings
2. Or update the action.yml to invalidate cache

### Missing Tools
The action verifies installation by checking:
- `dkp-pacman` command availability
- `powerpc-eabi-gcc` compiler availability
- `/opt/devkitpro` directory structure

## Supported Platforms

- ✅ Ubuntu (GitHub Actions runners)
- ✅ Debian-based Linux distributions
- ❌ Windows (use DevkitPro installer instead)
- ❌ macOS (use DevkitPro pkg installer instead)
