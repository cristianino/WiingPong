# Sistema de Sombras y Estelas Dinámicas - WiingPong

## Descripción General
El sistema de sombras y estelas dinámicas añade efectos visuales cinemáticos al juego, proporcionando profundidad, movimiento y dramatismo sin comprometer el rendimiento o la jugabilidad.

## Arquitectura del Sistema

### Componentes Principales

#### `PositionHistory`
Estructura para almacenar el historial de posiciones de la pelota:
```cpp
struct PositionHistory {
    float x, y;        // Posición en el frame
    float timestamp;   // Momento de captura
};
```

#### `EffectConfig`
Configuración completa de efectos por tipo de atlas:
```cpp
struct EffectConfig {
    // Configuración de sombras
    float shadowOffsetX, shadowOffsetY;
    float shadowOpacity;
    u32 shadowColor;
    
    // Configuración de estelas
    int maxTrailLength;
    float trailFadeRate;
    float trailSpacing;
    bool enableTrails;
    
    // Efectos dinámicos
    bool intensityBasedEffects;
    float glowIntensity;
};
```

## Efectos Implementados

### 1. Sombras Dinámicas

#### Sombras de Paletas
- **Offset Configurable**: Desplazamiento 3-5px diagonal
- **Transparencia**: 40-60% según atlas
- **Color Adaptativo**: Negro normal, cian oscuro intenso
- **Movimiento**: Estático normal, ondulante intenso

#### Sombras de Pelota
- **Seguimiento Preciso**: Sigue posición exacta de la pelota
- **Efecto Orgánico**: Movimiento sutil con funciones sin/cos
- **Escalado Proporcional**: Mantiene proporciones del sprite original

### 2. Sistema de Estelas de Movimiento

#### Generación de Estela
```cpp
// Solo genera estela cuando la pelota se mueve rápido
float speed = sqrt(velX² + velY²);
if (speed > 5.0f) {
    ballTrail.insert(ballTrail.begin(), 
                    PositionHistory(ballPos.x, ballPos.y, currentTime));
}
```

#### Renderizado de Estela
- **Múltiples Puntos**: 4-6 posiciones históricas
- **Fade Progresivo**: Alpha 1.0 → 0.1
- **Escalado Decreciente**: Tamaño 1.0 → 0.7
- **Colores Adaptativos**: Blanco normal, cian intenso

### 3. Efectos de Resplandor (Intense Mode)

#### Sistema de Capas Múltiples
```cpp
for (int layer = 0; layer < 3; layer++) {
    float layerScale = 1.0f + (layer * 0.3f);
    float layerAlpha = currentIntensity / (layer + 1);
    // Renderizar capa de resplandor
}
```

#### Pulsación Dinámica
- **Frecuencia**: 8Hz para efecto suave
- **Intensidad**: 50-100% con variación sinusoidal
- **Sincronización**: Basada en tiempo de juego real

## Configuraciones por Atlas

### Atlas Normal (Gameplay Regular)
```
Sombras:
├── Offset: 3px diagonal
├── Opacidad: 40%
├── Color: Negro (#000000)
└── Movimiento: Estático

Estelas:
├── Longitud: 4 puntos
├── Fade: 25% por punto
├── Color: Blanco normal
└── Activación: Velocidad > 5 unidades

Resplandor:
└── Deshabilitado (gameplay limpio)
```

### Atlas Intenso (Empate 3-3+)
```
Sombras:
├── Offset: 5px diagonal + movimiento
├── Opacidad: 60%
├── Color: Cian oscuro (#004444)
└── Movimiento: Ondulación sin/cos

Estelas:
├── Longitud: 6 puntos
├── Fade: 20% por punto
├── Color: Cian brillante
└── Activación: Velocidad > 5 unidades

Resplandor:
├── Intensidad: 30%
├── Capas: 3 concéntricas
├── Pulsación: 8Hz
└── Color: Cian luminoso
```

## Pipeline de Renderizado

### Orden de Capas (Z-Order)
1. **Background**: Capas parallax de fondo
2. **Shadows**: Sombras de paletas y pelota
3. **Ball Trail**: Estela de movimiento de la pelota
4. **Main Elements**: Paletas y pelota principales
5. **Glow Effects**: Resplandores (solo modo intenso)
6. **UI Elements**: Puntuación e información de debug

### Flujo de Renderizado
```cpp
// 1. Determinar configuración de efectos
EffectConfig config = getCurrentEffectConfig(currentAtlas);

// 2. Renderizar sombras
drawPaddleShadow(playerPos, playerSize, true, config);
drawPaddleShadow(cpuPos, cpuSize, false, config);
drawBallShadow(ballPos, ballSize, config);

// 3. Renderizar estela
if (config.enableTrails) {
    drawBallTrail(ballSize, config);
}

// 4. Renderizar elementos principales
drawPaddleSprite(playerPos, playerSize, true);
drawPaddleSprite(cpuPos, cpuSize, false);
drawBallSprite(ballPos, ballSize);

// 5. Renderizar resplandor (si aplicable)
if (config.glowIntensity > 0.0f) {
    drawGlowEffect(ballPos, ballSize, config, glowColor);
}
```

## Optimizaciones de Rendimiento

### Gestión de Memoria
- **Trail Limitado**: Máximo 6 puntos de historial
- **Cleanup Automático**: Eliminación de puntos antiguos (>1s)
- **Reutilización**: Uso de sprites existentes del atlas
- **Cálculos Eficientes**: Operaciones matemáticas optimizadas

### Condicionales Inteligentes
```cpp
// Solo genera trail cuando es necesario
if (speed > 5.0f) {
    // Añadir punto al trail
}

// Solo renderiza resplandor en modo intenso
if (config.glowIntensity > 0.0f) {
    // Renderizar efecto de resplandor
}

// Cleanup basado en tiempo
ballTrail.erase(std::remove_if(ballTrail.begin(), ballTrail.end(), 
               [this](const PositionHistory& point) {
                   return (currentTime - point.timestamp) > 1.0f;
               }), ballTrail.end());
```

### Cálculos Matemáticos
- **Funciones sin/cos**: Para movimiento orgánico de sombras
- **Interpolación Lineal**: Para transiciones suaves de alpha
- **Vectores Optimizados**: Para cálculos de velocidad

## Integración con Sistemas Existentes

### Sistema de Atlas
- **Detección Automática**: Cambio de efectos basado en atlas actual
- **Configuración Dinámica**: Parámetros ajustados automáticamente
- **Transiciones Suaves**: Sin interrupciones visuales

### Sistema de Parallax
- **Orden Coordinado**: Respeta el z-order del parallax
- **Compatibilidad Total**: No interfiere con capas de fondo
- **Rendimiento Conjunto**: Optimizado para trabajar en conjunto

### Sistema de Física
- **Tracking de Velocidad**: Usa datos reales de PhysicsEngine
- **Posicionamiento Preciso**: Sincronizado con posiciones exactas
- **Detección de Movimiento**: Basado en velocidad calculada

## API de Configuración

### Métodos Principales
```cpp
// Configuración inicial
void setupEffectConfigurations();

// Obtener configuración actual
EffectConfig getCurrentEffectConfig(AtlasType currentAtlas) const;

// Actualizar trail de pelota
void updateBallTrail(const Position& ballPos, const Velocity& ballVel);

// Renderizado de efectos
void drawPaddleShadow(const Position& pos, const Size& size, 
                     bool isLeftPaddle, const EffectConfig& config);
void drawBallShadow(const Position& pos, const Size& size, 
                   const EffectConfig& config);
void drawBallTrail(const Size& ballSize, const EffectConfig& config);
void drawGlowEffect(const Position& pos, const Size& size, 
                   const EffectConfig& config, u32 glowColor);
```

### Parámetros Ajustables
```cpp
// Configuración de sombras
config.shadowOffsetX = 3.0f;      // Desplazamiento horizontal
config.shadowOffsetY = 3.0f;      // Desplazamiento vertical
config.shadowOpacity = 0.4f;      // Transparencia (0.0-1.0)
config.shadowColor = 0x000000FF;  // Color RGBA

// Configuración de estelas
config.maxTrailLength = 4;        // Número máximo de puntos
config.trailFadeRate = 0.25f;     // Velocidad de desvanecimiento
config.trailSpacing = 0.7f;       // Espaciado entre puntos
config.enableTrails = true;       // Habilitar/deshabilitar

// Efectos dinámicos
config.intensityBasedEffects = true;  // Efectos basados en intensidad
config.glowIntensity = 0.0f;          // Intensidad de resplandor
```

## Casos de Uso y Escenarios

### Gameplay Normal
- **Sombras Sutiles**: Añaden profundidad sin distraer
- **Estelas Ocasionales**: Solo durante movimientos rápidos
- **Estética Limpia**: Mantiene claridad visual

### Modo Intenso (3-3+ Empate)
- **Drama Visual**: Efectos intensificados para tensión
- **Feedback Visual**: Resplandores y movimiento dinámico
- **Ambiente Cyberpunk**: Colores cian y efectos futuristas

### Detección de Movimiento
- **Umbral de Velocidad**: >5 unidades para activar trail
- **Respuesta Inmediata**: Efectos reactivos al gameplay
- **Limpieza Automática**: Removal de trails cuando se detiene

## Futuras Expansiones Posibles

### Efectos Adicionales
- **Partículas de Impacto**: En colisiones con paletas/paredes
- **Ondas de Choque**: Efectos radiales en eventos especiales
- **Distorsión Temporal**: Slow-motion con estelas extendidas

### Personalización Avanzada
- **Perfiles de Efectos**: Múltiples configuraciones guardadas
- **Intensidad Adjustable**: Control fino de todos los parámetros
- **Efectos Estacionales**: Temas especiales por temporada

### Optimizaciones Futuras
- **LOD de Efectos**: Diferentes niveles de detalle
- **Batching de Renders**: Optimización de llamadas de renderizado
- **Shaders Personalizados**: Efectos más avanzados si el hardware lo permite

El sistema de sombras y estelas transforma WiingPong de un juego funcional a una experiencia visual cinematográfica, manteniendo el espíritu clásico pero añadiendo el pulido visual de los títulos modernos.