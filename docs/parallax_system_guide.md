# Sistema de Parallax Background - WiingPong

## Descripción General
El sistema de parallax background añade profundidad visual y dinamismo al juego mediante múltiples capas de fondo que se mueven a diferentes velocidades, creando una sensación de profundidad 3D en un juego 2D.

## Arquitectura del Sistema

### Componentes Principales

#### `BackgroundManager`
Clase principal que controla todo el sistema de parallax:
- Gestiona múltiples capas con diferentes velocidades
- Aplica efectos visuales dinámicos
- Reacciona a eventos del gameplay
- Optimizado para 60 FPS en 480p

#### `ParallaxLayer`
Estructura que define cada capa individual:
```cpp
struct ParallaxLayer {
    SpriteID spriteId;        // Sprite a usar
    float scrollSpeedX;       // Velocidad horizontal
    float scrollSpeedY;       // Velocidad vertical
    float opacity;            // Transparencia (0.0-1.0)
    bool wrapHorizontal;      // Repetición horizontal
    bool wrapVertical;        // Repetición vertical
    float pulseSpeed;         // Velocidad de pulsación
    float vibrateIntensity;   // Intensidad de vibración
};
```

#### `BackgroundLayer` (Enum)
Define los tipos de capas por profundidad:
- `FarBackground`: Capa más lejana (movimiento lento)
- `MidBackground`: Capa intermedia (movimiento medio)
- `NearBackground`: Capa cercana (movimiento rápido)
- `PlayingField`: Campo de juego (estático o mínimo)

## Configuraciones por Atlas

### Atlas Normal (Juego Regular)
```
Far Layer:    Speed 0.1x, Opacity 30%, Pulso sutil
Mid Layer:    Speed 0.3x, Opacity 50%, Vibración ligera
Near Layer:   Speed 0.8x, Opacity 70%, Efectos activos
Playing Field: Estático,   Opacity 100%, Pulso mínimo
```

### Atlas Intenso (Empate 3-3+)
```
Far Layer:    Speed 0.3x, Opacity 40%, Pulso activo
Mid Layer:    Speed 0.6x, Opacity 60%, Vibración fuerte
Near Layer:   Speed 1.2x, Opacity 80%, Efectos intensos
Playing Field: Speed 0.05x, Opacity 100%, Movimiento notable
```

## Efectos Visuales Implementados

### 1. Desplazamiento Parallax
```cpp
float offset = frameCounter * layer.scrollSpeedX;
```
- Cada capa se mueve a diferente velocidad
- Crea ilusión de profundidad 3D
- Movimiento suave y continuo

### 2. Vibración (Micro-movimientos)
```cpp
float vibration = sin(time * 15.0f) * intensity * 0.5f;
```
- Movimientos sutiles de alta frecuencia
- Intensidad configurable por capa
- Perfectamente visible en 480p

### 3. Pulsación (Breathing Effect)
```cpp
float pulse = (sin(time * speed) + 1.0f) * 0.5f;
```
- Cambio gradual de opacidad
- Efecto de "respiración" orgánico
- Velocidad ajustable por capa

### 4. Efectos Reactivos al Gameplay

#### Detección de Movimiento de Pelota
```cpp
float ballSpeed = sqrt(velX² + velY²);
if (ballSpeed > 10.0f) {
    // Añadir ondas sutiles
    offset += sin(frameCounter * 0.5f) * 0.5f;
}
```

#### Reacción a Eventos de Puntuación
```cpp
if (recentScoreEvent) {
    float intensity = scoreEventTimer / 2.0f;
    offset += sin(frameCounter * 2.0f) * 3.0f * intensity;
}
```

## Optimizaciones para 480p

### Precisión de Movimiento
- Movimientos de 0.5px son perfectamente visibles
- Frecuencias altas (15Hz) para vibración suave
- Velocidades calibradas para resolución nativa

### Eficiencia de Renderizado
- Reutilización de sprites del atlas existente
- Cálculos optimizados por frame
- Wrapping inteligente para evitar overdraw

### Gestión de Memoria
- Sin texturas adicionales requeridas
- Uso eficiente de los atlas existentes
- Estructuras ligeras para configuración

## API de Uso

### Inicialización
```cpp
BackgroundManager backgroundManager;
backgroundManager.init();
```

### Update Loop
```cpp
// En el game loop principal
backgroundManager.update(deltaTime, physics);
```

### Renderizado
```cpp
// Antes de renderizar elementos del juego
backgroundManager.render(currentAtlas);
```

### Control Dinámico
```cpp
// Cambiar velocidad de animación
backgroundManager.setAnimationSpeed(1.5f);

// Efectos manuales
backgroundManager.setGameplayEffects(ballMoving, justScored);

// Reset completo
backgroundManager.reset();
```

## Integración con Sistema de Atlas

### Normal Atlas
- Parallax sutil y relajado
- Colores neutros (blanco/normal)
- Efectos mínimos para no distraer

### Intense Atlas (3-3+ empate)
- Parallax agresivo y dinámico
- Tinte cyan/turquesa
- Efectos intensificados
- Mayor velocidad de movimiento

## Consideraciones de Diseño

### Performance
- Optimizado para hardware Wii
- 60 FPS constantes mantenidos
- Uso mínimo de CPU adicional

### Escalabilidad
- Fácil añadir nuevas capas
- Configuraciones por atlas extensibles
- Efectos modulares

### Compatibilidad
- Fallback graceful si falla carga
- Compatible con sistema de atlas existente
- No rompe funcionalidad anterior

## Casos de Uso Avanzados

### Eventos Especiales
- Posibilidad de añadir efectos para power-ups
- Reacciones a combos o jugadas especiales
- Transiciones entre estados de juego

### Futuras Extensiones
- Parallax vertical para mayor dinamismo
- Partículas sincronizadas con parallax
- Efectos de profundidad de campo
- Animaciones de transición entre atlas

## Debugging y Testing

### Scripts de Prueba
- `test_parallax_system.sh`: Verificación completa del sistema
- Validación de archivos y configuración
- Reporte de características implementadas

### Parámetros Ajustables
- Velocidades de parallax por capa
- Intensidades de efectos
- Umbrales de detección de gameplay
- Tiempos de duración de efectos

El sistema de parallax transforma WiingPong de un juego estático a una experiencia visual dinámica y envolvente, manteniendo la jugabilidad clásica pero añadiendo la profundidad visual moderna.