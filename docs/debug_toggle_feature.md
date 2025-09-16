# Toggle de Debug View - WiingPong

## Nueva Funcionalidad Implementada

Se añadió la capacidad de habilitar/deshabilitar la ventana de debug durante el juego usando el **Botón 1** del Wiimote.

## Controles Actualizados

### Controles de Juego:
- **Botón A**: Mover barra hacia arriba
- **Botón B**: Mover barra hacia abajo  
- **Botón HOME**: Salir del juego

### Controles de Debug:
- **Botón 1**: Toggle (habilitar/deshabilitar) ventana de debug

## Cómo Funciona

### Estado Inicial
- La ventana de debug está **deshabilitada** por defecto
- El juego se ve limpio sin elementos de debug

### Activar Debug
1. Presiona el **Botón 1** del Wiimote
2. La ventana de debug aparece en la esquina inferior izquierda
3. Se muestra un mensaje en consola: "Debug view enabled"

### Desactivar Debug
1. Presiona el **Botón 1** del Wiimote nuevamente
2. La ventana de debug desaparece
3. Se muestra un mensaje en consola: "Debug view disabled"

## Indicador Visual

Cuando la ventana de debug está visible, hay un pequeño indicador en la esquina superior derecha:
- **Cuadrado gris** - Botón 1 no presionado
- **Cuadrado blanco** - Botón 1 presionado (momento del toggle)
- **Borde blanco** - Indica que es el botón de toggle

## Implementación Técnica

### Cambios en el Código:

1. **InputManager**: 
   - Añadido `InputEventType::ToggleDebug`
   - Detección de `WPAD_BUTTON_1`

2. **Renderer**:
   - Variable `debugVisible` para controlar visibilidad
   - Funciones `setDebugVisible()` e `isDebugVisible()`
   - Indicador visual del botón toggle

3. **Main Loop**:
   - Procesamiento del evento `ToggleDebug`
   - Mensajes de estado en consola

### Flujo de Ejecución:
1. Se presiona Botón 1
2. Se genera evento `ToggleDebug`
3. Se invierte el estado de `debugVisible`
4. Se muestra/oculta la ventana en el siguiente frame

## Beneficios

### Para Desarrollo:
- Debug disponible cuando se necesita
- Juego limpio para presentación/distribución
- Toggle rápido sin reiniciar

### Para Usuarios:
- Interfaz limpia por defecto
- Opción de ver información técnica si es necesario
- Control total sobre la experiencia visual

## Uso Recomendado

- **Durante desarrollo**: Habilitar debug para diagnosticar problemas
- **Para release**: Los usuarios pueden activarlo si tienen problemas de conectividad
- **Presentaciones**: Mantener deshabilitado para experiencia limpia
