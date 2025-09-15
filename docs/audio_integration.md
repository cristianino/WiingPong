# Integración de Audio en WiingPong

## Resumen
Se ha implementado soporte completo para reproducir archivos de audio PCM en WiingPong. El archivo `intro.pcm` se reproduce automáticamente al iniciar el juego.

## Componentes Implementados

### AudioManager
- **Ubicación**: `include/audio/AudioManager.h` y `source/audio/AudioManager.cpp`
- **Funcionalidad**: 
  - Inicialización de la librería ASND de Wii
  - Carga de archivos PCM desde disco
  - Reproducción de sonidos y música
  - Gestión de memoria para buffers de audio

### AssetManager
- **Actualización**: Se agregó el método `loadAudio()` para cargar archivos de audio
- **Integración**: Carga automática del archivo `intro.pcm` al inicializar

### main.cpp
- **Inicialización**: Se agregó la inicialización del AudioManager
- **Carga de Assets**: Se llama a `loadAudio()` para cargar los archivos de sonido
- **Reproducción**: Se reproduce automáticamente el sonido de intro al iniciar

## Archivos de Audio Soportados

### intro.pcm
- **Ubicación**: `data/sounds/intro.pcm`
- **Formato**: PCM Raw (16-bit mono, 22050 Hz por defecto)
- **Uso**: Se reproduce automáticamente al iniciar el juego

## Estructura de Archivos

```
WiingPong/
├── data/
│   └── sounds/
│       └── intro.pcm        # Archivo de audio de introducción
├── include/
│   └── audio/
│       └── AudioManager.h   # Header del gestor de audio
├── source/
│   └── audio/
│       └── AudioManager.cpp # Implementación del gestor de audio
└── apps/
    └── WiingPong/
        ├── boot.dol         # Ejecutable del juego
        ├── meta.xml         # Metadata de la aplicación
        └── data/
            └── sounds/
                └── intro.pcm # Audio incluido en el paquete
```

## Compilación y Empaquetado

El Makefile ha sido actualizado para:
1. Incluir la librería ASND para audio (`-lasnd`)
2. Copiar automáticamente la carpeta `data/` al paquete de la aplicación
3. Crear releases comprimidos que incluyen todos los archivos necesarios

### Comandos
```bash
make clean      # Limpiar archivos de compilación
make            # Compilar el proyecto
make package    # Crear paquete para SD card
make release    # Crear archivos comprimidos para distribución
```

## Agregar Más Sonidos

Para agregar nuevos sonidos:

1. **Agregar el enum**: En `AudioManager.h`, agregar un nuevo valor al enum `SoundID`
2. **Colocar el archivo**: Guardar el archivo PCM en `data/sounds/`
3. **Cargar el audio**: En `AssetManager.cpp`, agregar una línea en `loadAudio()`:
   ```cpp
   audioManager.loadPCM(SoundID::NuevoSonido, "data/sounds/nuevo_sonido.pcm");
   ```
4. **Reproducir**: Usar `audio.playSound(SoundID::NuevoSonido)` donde sea necesario

## Notas Técnicas

- **Formato PCM**: Los archivos deben estar en formato PCM raw (sin headers)
- **Frecuencia**: 22050 Hz por defecto, configurable por archivo
- **Canales**: Mono (16-bit) por defecto
- **Memoria**: Los archivos se cargan completamente en RAM
- **Limitaciones**: ASND soporta hasta 4 voces simultáneas

## Instalación

1. Copia la carpeta `apps/` a la raíz de tu tarjeta SD
2. El archivo `intro.pcm` se incluye automáticamente en el paquete
3. Ejecuta WiingPong desde el Homebrew Channel

El sonido de introducción se reproducirá automáticamente al iniciar el juego.
