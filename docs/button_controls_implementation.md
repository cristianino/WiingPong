# Implementación de Controles con Botones A y B del Wiimote

## Cambios Realizados

### 1. Actualización del InputManager

Se modificaron los archivos para usar los botones A y B del Wiimote en lugar de los botones PLUS y MINUS:

#### `include/input/InputManager.h`
- Actualizado comentario: "A button held for player movement up"
- Actualizado comentario: "B button held for player movement down"

#### `source/input/InputManager.cpp`
- Cambiado `WPAD_BUTTON_PLUS` por `WPAD_BUTTON_A` para movimiento hacia arriba
- Cambiado `WPAD_BUTTON_MINUS` por `WPAD_BUTTON_B` para movimiento hacia abajo

### 2. Controles del Juego

**Nuevos controles:**
- **Botón A**: Mover barra del jugador hacia arriba
- **Botón B**: Mover barra del jugador hacia abajo  
- **Botón HOME**: Salir del juego

### 3. Estructura del Homebrew

Se creó la estructura correcta para el Homebrew Channel:
- `apps/WiingPong/boot.dol` - Ejecutable del juego
- `apps/WiingPong/meta.xml` - Metadatos para el Homebrew Channel
- `apps/WiingPong/data/` - Archivos de audio

### 4. Compilación

El proyecto se compila exitosamente con los nuevos controles:
```bash
make clean && make
```

### 5. Funcionalidad

Los botones A y B ahora controlan el movimiento de la barra del jugador:
- **Mantener presionado A**: Movimiento continuo hacia arriba
- **Mantener presionado B**: Movimiento continuo hacia abajo
- **Soltar botones**: La barra se detiene

El sistema mantiene la misma velocidad de movimiento y la misma respuesta que los controles anteriores.
