# Guía de Releases Manual para WiingPong

## 🔧 Si el Release Automático Falla

### Pasos para Crear Release Manual

1. **Ve a GitHub Releases:**
   ```
   https://github.com/cristianino/WiingPong/releases/new
   ```

2. **Configurar el Release:**
   - **Tag version**: Usa el tag que creaste (ej: `v1.1.1`)
   - **Release title**: `WiingPong v1.1.1`
   - **Target**: `master` (o la rama correcta)

3. **Descripción del Release:**
   ```markdown
   ## WiingPong v1.1.1
   
   🎮 **Audio-enabled Pong game for Nintendo Wii**
   
   ### Features
   - Classic Pong gameplay with Wiimote controls
   - **NEW**: Intro audio automatically plays on startup
   - Extensible audio system using ASND library
   - Clean, modular C++ codebase
   
   ### What's Included
   - Complete source code
   - Audio files (intro.pcm)
   - Build system (Makefile)
   - Documentation
   
   ### Installation
   1. **Build Requirements**: devkitPro with devkitPPC toolchain
   2. **Build**: `make clean && make package`
   3. **Install**: Copy `apps/` folder to SD card root
   4. **Play**: Launch from Homebrew Channel
   
   ### Audio System
   This release includes a complete audio system:
   - PCM audio file support
   - Automatic intro sound playback
   - Memory-managed audio buffers
   - Easy to extend with more sounds
   
   **Repository**: https://github.com/cristianino/WiingPong
   **Documentation**: See `docs/` folder for detailed setup and audio integration guides
   ```

4. **Archivos a Subir:**
   
   Si el CI corrió, descarga los artefactos de GitHub Actions:
   - `WiingPong-v1.1.1-source.tar.gz`
   - `release-info.txt`
   
   Si no, crea el paquete localmente:
   ```bash
   # Crear paquete de código fuente
   git archive --format=tar.gz --prefix=WiingPong-v1.1.1/ v1.1.1 > WiingPong-v1.1.1-source.tar.gz
   
   # O hacer un build completo y paquete
   make clean
   make package
   make release
   ```

## 🚀 Releases Automáticos (Esperado)

### Con las Correcciones Aplicadas:
- ✅ Permisos de workflow: `contents: write`
- ✅ Manejo de errores mejorado
- ✅ Fallback con GitHub CLI
- ✅ Artefactos siempre disponibles

### Para Crear Nuevos Releases:
```bash
# Crear tag anotado
git tag v1.2.0 -m "Descripción de la versión"

# Subir tag (activa el workflow)
git push origin v1.2.0

# El CI automáticamente debería:
# 1. Crear el release en GitHub
# 2. Subir archivos de código fuente
# 3. Generar notas de release
```

## 🔍 Debugging de Releases

### Verificar el Estado del Workflow:
1. Ve a: `https://github.com/cristianino/WiingPong/actions`
2. Busca el workflow ejecutado por tu tag
3. Revisa los logs de cada paso

### Problemas Comunes:

#### Error 403 (Permisos):
- ✅ **Solucionado**: Agregamos `permissions: contents: write`

#### Error de Token:
```yaml
# Ya incluido en el workflow
env:
  GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
```

#### Archivos No Encontrados:
- Los artefactos están disponibles incluso si falla el release
- Descargar desde la pestaña "Artifacts" del workflow

### Fallbacks Implementados:

1. **Primary**: softprops/action-gh-release@v2
2. **Secondary**: GitHub CLI (gh)
3. **Manual**: Instrucciones en workflow summary + artifacts

## 📦 Verificación Post-Release

Después de crear un release (manual o automático):

1. **Verificar en GitHub**:
   - Release aparece en `/releases`
   - Archivos están adjuntos
   - Descripción es correcta

2. **Probar Descarga**:
   ```bash
   wget https://github.com/cristianino/WiingPong/archive/v1.1.1.tar.gz
   tar -xzf v1.1.1.tar.gz
   cd WiingPong-1.1.1
   ```

3. **Verificar Contenido**:
   - Código fuente completo
   - Archivo `data/sounds/intro.pcm` incluido
   - Documentación presente
   - Makefile funcional

## 🎯 Estado Actual

Con las correcciones aplicadas en v1.1.1:
- ✅ Permisos corregidos
- ✅ Múltiples fallbacks implementados
- ✅ Mejor manejo de errores
- ✅ Artefactos siempre disponibles
- ✅ Instrucciones de emergencia incluidas

**El sistema debería funcionar automáticamente a partir de ahora.** 🚀
