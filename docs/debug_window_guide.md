# Ventana de Debug Visual para Wiimote - WiingPong

## ¿Qué se Implementó?

Se añadió una ventana de debug visual (sin texto) en la pantalla que muestra información en tiempo real sobre el estado del Wiimote y los botones presionados usando elementos gráficos.

## Información Mostrada Visualmente

### 1. Estado de Inicialización WPAD
- **Cuadrado Verde** (esquina superior izquierda) - WPAD está inicializado
- **Cuadrado Rojo** (esquina superior izquierda) - WPAD no inicializado

### 2. Indicadores de Botones Individuales
Fila de cuadrados de colores que representan cada botón:
- **Cuadrado Verde** - Botón A presionado
- **Cuadrado Azul** - Botón B presionado  
- **Cuadrado Rojo** - Botón HOME presionado
- **Cuadrado Amarillo** - Botón PLUS presionado
- **Cuadrado Magenta** - Botón MINUS presionado
- **Cuadrado Gris Oscuro** - Botón no presionado

### 3. Representación Hexadecimal Visual
Dos filas de pequeños rectángulos que representan los bits:
- **Fila Superior**: Botones mantenidos presionados (held)
- **Fila Inferior**: Botones recién presionados (pressed)
- **Rectángulo Blanco**: Bit activado (1)
- **Rectángulo Gris Oscuro**: Bit desactivado (0)

## Ubicación en Pantalla

La ventana de debug aparece en la parte inferior izquierda con:
- Fondo semi-transparente negro con borde blanco
- Indicador de estado en la esquina superior izquierda
- Fila de indicadores de botones en el centro
- Representación de bits en la parte inferior

## Interpretación Visual

### Estado Normal (Sin botones presionados):
- 1 cuadrado verde (WPAD inicializado)
- 5 cuadrados grises (botones A, B, HOME, +, -)
- Rectángulos grises en las filas de bits

### Cuando presionas el Botón A:
- Cuadrado verde (WPAD)
- Primer cuadrado verde (A), resto grises
- Bit correspondiente a A (0x0008) aparece blanco en la fila superior

### Cuando presionas el Botón B:
- Cuadrado verde (WPAD)  
- Segundo cuadrado azul (B), resto grises
- Bit correspondiente a B (0x0004) aparece blanco en la fila superior

## Ventajas del Sistema Visual

1. **Sin dependencias de texto**: No requiere funciones de texto no disponibles
2. **Información inmediata**: Los colores cambian instantáneamente
3. **Representación precisa**: Cada bit se puede visualizar individualmente
4. **Compatibilidad total**: Funciona con cualquier versión de GRRLIB

## Diagnóstico de Problemas

1. **WPAD no funciona**: Cuadrado rojo en lugar de verde
2. **Botones no responden**: Los cuadrados de colores no cambian
3. **Detección parcial**: Solo algunos botones cambian de color
4. **Valores incorrectos**: Los patrones de bits no coinciden con los esperados

## Valores de Referencia para los Bits

Cuando funciona correctamente, presionar cada botón activará estos bits:
- **A**: Bit 3 (0x0008)
- **B**: Bit 2 (0x0004)
- **HOME**: Bit 7 (0x0080)
- **PLUS**: Bit 4 (0x0010)  
- **MINUS**: Bit 12 (0x1000)

## Ubicación en Pantalla

La ventana de debug aparece en la parte inferior izquierda de la pantalla con:
- Fondo semi-transparente negro
- Borde blanco
- Texto blanco para la mayoría de información
- Texto verde para "CONNECTED"
- Texto rojo para "DISCONNECTED"

## Mejoras en la Inicialización

Se mejoró la función de inicialización del InputManager para:
- Esperar un tiempo después de inicializar WPAD
- Buscar activamente el Wiimote durante 1 segundo
- Mostrar mensajes de debug en la consola

## Diagnóstico de Problemas

Con esta ventana podrás determinar:

1. **Si el Wiimote se conecta**: Mira el estado de conexión
2. **Si los botones funcionan**: Observa los valores hex y estados individuales
3. **Qué botones están siendo detectados**: Compara los valores esperados
4. **Si hay problemas de inicialización**: Revisa el estado "Init"

## Valores Hexadecimales de Referencia

Cuando presiones botones, deberías ver estos valores aproximados:
- **WPAD_BUTTON_A**: 0x0008
- **WPAD_BUTTON_B**: 0x0004  
- **WPAD_BUTTON_HOME**: 0x0080
- **WPAD_BUTTON_PLUS**: 0x0010
- **WPAD_BUTTON_MINUS**: 0x1000

## Uso

1. Ejecuta el juego en el Wii/Dolphin
2. Observa la ventana de debug en la esquina inferior izquierda
3. Presiona botones en el Wiimote
4. Verifica que los valores cambien en la pantalla
5. Si no cambian, el problema está en la detección del Wiimote
