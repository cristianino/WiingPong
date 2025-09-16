# WiingPong - Sistema de Configuración

## Configuración en Tiempo de Compilación

WiingPong utiliza un sistema de configuración basado en `#define` que permite personalizar el comportamiento del juego sin modificar el código fuente principal.

## Archivo de Configuración

La configuración se encuentra en: **`include/config.h`**

## Opciones de Configuración Disponibles

### 🔧 **Configuración de Debug**

#### Habilitar/Deshabilitar Debug
```c
#define WIINGPONG_DEBUG_ENABLED 1    // 1 = habilitado, 0 = deshabilitado
```

#### Método de Toggle del Debug
```c
#define WIINGPONG_DEBUG_TOGGLE_METHOD 1
// 0 = Presión simple de botón (Button 1, 2, +, -)
// 1 = Mantener A+B por tiempo específico
```

#### Duración de A+B (en frames a 60fps)
```c
#define WIINGPONG_DEBUG_TOGGLE_DURATION_FRAMES 240
// 240 frames = 4 segundos
// 180 frames = 3 segundos  
// 300 frames = 5 segundos
```

#### Botón para Método Simple
```c
#define WIINGPONG_DEBUG_TOGGLE_BUTTON WPAD_BUTTON_1
// Opciones: WPAD_BUTTON_1, WPAD_BUTTON_2, WPAD_BUTTON_PLUS, WPAD_BUTTON_MINUS
```

### 🎮 **Configuración de Juego**

#### Velocidad del Paddle
```c
#define WIINGPONG_PADDLE_SPEED 5
// Velocidad de movimiento del paddle del jugador
```

#### Velocidad de la Pelota
```c
#define WIINGPONG_BALL_SPEED_X 3    // Velocidad horizontal
#define WIINGPONG_BALL_SPEED_Y 2    // Velocidad vertical
```

### 🔊 **Configuración de Audio**

#### Habilitar Audio
```c
#define WIINGPONG_AUDIO_ENABLED 1           // Audio TV
#define WIINGPONG_WIIMOTE_AUDIO_ENABLED 1   // Audio Wiimote
```

### 📦 **Información de Versión**

```c
#define WIINGPONG_VERSION_MAJOR 1
#define WIINGPONG_VERSION_MINOR 2
#define WIINGPONG_VERSION_PATCH 0
```

## Configuraciones Predefinidas

### Configuración de Desarrollo (Actual)
```c
#define WIINGPONG_DEBUG_ENABLED 1
#define WIINGPONG_DEBUG_TOGGLE_METHOD 1
#define WIINGPONG_DEBUG_TOGGLE_DURATION_FRAMES 240  // 4 segundos
```

### Configuración de Release
```c
#define WIINGPONG_DEBUG_ENABLED 0
// (Todo el código de debug se elimina en compilación)
```

### Configuración de Debug Rápido
```c
#define WIINGPONG_DEBUG_ENABLED 1
#define WIINGPONG_DEBUG_TOGGLE_METHOD 0
#define WIINGPONG_DEBUG_TOGGLE_BUTTON WPAD_BUTTON_1
```

## Cómo Usar las Configuraciones

### 1. **Editar Configuración**
```bash
nano include/config.h
# Modifica los valores deseados
```

### 2. **Compilar con Nueva Configuración**
```bash
make clean && make
# La nueva configuración se aplica automáticamente
```

### 3. **Crear Release**
```bash
make package && make release
```

## Validaciones Automáticas

El sistema incluye validaciones que previenen configuraciones inválidas:

```c
#if WIINGPONG_DEBUG_ENABLED && (WIINGPONG_DEBUG_TOGGLE_METHOD < 0 || WIINGPONG_DEBUG_TOGGLE_METHOD > 1)
#error "WIINGPONG_DEBUG_TOGGLE_METHOD must be 0 or 1"
#endif
```

## Ejemplos de Uso

### Debug Deshabilitado (Release)
1. Cambiar `WIINGPONG_DEBUG_ENABLED` a `0`
2. Compilar: `make clean && make`
3. Resultado: Juego sin código de debug, más pequeño y rápido

### Activación Rápida de Debug
1. Cambiar `DEBUG_TOGGLE_METHOD` a `0`
2. Configurar `DEBUG_TOGGLE_BUTTON` a `WPAD_BUTTON_PLUS`
3. Resultado: Debug se activa presionando + una vez

### Tiempo Personalizado de A+B
1. Cambiar `DEBUG_TOGGLE_DURATION_FRAMES` a `180` (3 segundos)
2. Resultado: Debug se activa más rápido

## Ventajas del Sistema

- **Sin modificar código**: Solo editas un archivo de configuración
- **Compilación condicional**: Código innecesario se elimina automáticamente
- **Validación**: Previene configuraciones inválidas
- **Flexibilidad**: Múltiples opciones para diferentes necesidades
- **Mantenibilidad**: Configuración centralizada y documentada
