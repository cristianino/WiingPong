# Toggle de Debug View con A+B - WiingPong

## Nueva Funcionalidad Implementada

Se añadió la capacidad de habilitar/deshabilitar la ventana de debug durante el juego manteniendo presionados **A+B durante 4 segundos**.

## Controles Actualizados

### Controles de Juego:
- **Botón A**: Mover barra hacia arriba (cuando no se usa para debug)
- **Botón B**: Mover barra hacia abajo (cuando no se usa para debug)
- **Botón HOME**: Salir del juego

### Controles de Debug:
- **A + B (mantener 4 segundos)**: Toggle (habilitar/deshabilitar) ventana de debug

## Cómo Funciona

### Estado Inicial
- La ventana de debug está **deshabilitada** por defecto
- El juego se ve limpio sin elementos de debug

### Activar Debug
1. **Mantén presionados A+B** simultáneamente
2. Aparece una **barra de progreso** en la parte superior de la pantalla
3. **Indicadores laterales** muestran que A y B están presionados (verde/azul)
4. Después de **4 segundos**, la ventana de debug aparece
5. Se muestra mensaje en consola: "Debug view enabled"

### Desactivar Debug
1. **Mantén presionados A+B** nuevamente durante 4 segundos
2. La barra de progreso aparece otra vez
3. Al completarse, la ventana de debug desaparece
4. Se muestra mensaje en consola: "Debug view disabled"

## Indicadores Visuales

### Barra de Progreso
Cuando mantienes A+B, aparece en la parte superior:
- **Fondo negro** con borde blanco
- **Barra verde** que se llena progresivamente
- **Barra amarilla** cuando está a punto de completarse (80%+)
- **Indicador A** (lado izquierdo) - verde cuando presionado
- **Indicador B** (lado derecho) - azul cuando presionado

### Durante el Progreso
- Los controles de movimiento del paddle se **desactivan** mientras mantienes A+B
- Esto previene movimientos accidentales durante la activación del debug

## Implementación Técnica

### Timing System:
- **240 frames** a 60fps = 4 segundos exactos
- **Timer reset** si sueltas cualquier botón antes de completar
- **Prevención de triggers múltiples** con reset automático

### Estados del Sistema:
1. **Normal**: A y B funcionan como controles individuales
2. **Detectando**: A+B presionados, iniciando timer
3. **Progresando**: Mostrando barra de progreso
4. **Completado**: Toggle ejecutado, reset del timer

### Visual Feedback:
- **Tiempo real**: La barra se llena conforme pasa el tiempo
- **Estado de botones**: Indicadores laterales cambian de color
- **Prevención accidental**: Requiere acción deliberada y sostenida

## Beneficios del Nuevo Sistema

### Prevención de Activación Accidental:
- **4 segundos** es tiempo suficiente para ser intencional
- **Barra de progreso** da feedback claro del proceso
- **Combinación A+B** es menos probable de presionar por accidente

### Experiencia de Usuario:
- **Feedback visual inmediato** cuando se detecta A+B
- **Control preciso** del timing del toggle
- **No interfiere** con los controles normales del juego

### Robustez:
- **Reset automático** si se sueltan los botones antes de tiempo
- **Una sola función** por combinación completa
- **Estado claro** del progreso en todo momento

## Uso Recomendado

- **Durante juego normal**: Los botones A/B funcionan normalmente para el paddle
- **Para debug**: Mantén A+B deliberadamente por 4 segundos
- **Troubleshooting**: Activar cuando hay problemas de conectividad
- **Desarrollo**: Toggle cuando necesites diagnosticar el estado del Wiimote
