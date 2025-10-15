# Sistema de Atlas Múltiples - WiingPong

## Descripción
El sistema de atlas múltiples permite cambiar dinámicamente los gráficos del juego basándose en el estado de la partida. Cuando los jugadores están empatados a partir de 3-3, el juego cambia automáticamente a un atlas más intenso con efectos visuales especiales.

## Archivos de Atlas

### Atlas Normal (`atlas.png` / `spritesheet.png`)
- Usado durante el juego regular
- Gráficos estándar del juego
- Colores normales y efectos básicos

### Atlas Intenso (`atlas_2.png`)
- Activado durante empates 3-3 o superiores
- Temática cyberpunk/futurista
- Colores cyan/turquesa brillantes
- Efectos de neón y brillos

## Lógica de Activación

```cpp
// Se activa el atlas intenso cuando:
if (playerScore >= 3 && cpuScore >= 3 && playerScore == cpuScore) {
    // Usar Atlas Intenso
} else {
    // Usar Atlas Normal
}
```

## Efectos Especiales del Modo Intenso

1. **Bordes Pulsantes**: Bordes cyan que pulsan alrededor de la pantalla
2. **Brillos en Esquinas**: Efectos de resplandor en las cuatro esquinas
3. **Línea Central Mejorada**: Brillo especial en la línea del centro
4. **Fondo Más Oscuro**: Para crear más contraste con los elementos brillantes

## Implementación Técnica

### Clases Principales
- `AssetManager`: Maneja la carga y cambio de atlas
- `Renderer`: Renderiza con el atlas activo y efectos especiales
- `AtlasType`: Enum que define Normal e Intense

### Métodos Clave
```cpp
// Determinar atlas basado en puntajes
AtlasType determineAtlasForScores(int playerScore, int cpuScore);

// Cambiar atlas activo
void setCurrentAtlas(AtlasType atlasType);

// Obtener atlas específico
GRRLIB_texImg* getAtlas(AtlasType atlasType);

// Efectos especiales para modo intenso
void drawIntenseEffects();
```

## Instalación

1. Coloca `atlas.png` en `data/img/` para gráficos normales
2. Coloca `atlas_2.png` en `data/img/` para gráficos intensos
3. El juego detectará automáticamente ambos archivos
4. Si falta algún atlas, usa fallbacks automáticos

## Compatibilidad

- ✅ Totalmente compatible con el sistema anterior
- ✅ Fallback automático a `spritesheet.png` si no encuentra atlas
- ✅ Funciona sin `atlas_2.png` (solo modo normal)
- ✅ Múltiples rutas de carga para diferentes despliegues

## Extensiones Futuras

- Añadir más tipos de atlas (ej: AtlasType::Championship)
- Efectos sonoros especiales para cada atlas
- Transiciones animadas entre atlas
- Atlas dinámicos basados en otros eventos del juego