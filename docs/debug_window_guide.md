# Ventana de Debug para Wiimote - WiingPong

## ¿Qué se Implementó?

Se añadió una ventana de debug superpuesta en la pantalla que muestra información en tiempo real sobre el estado del Wiimote y los botones presionados.

## Información Mostrada

### 1. Estado de Inicialización WPAD
- **"WPAD: INITIALIZED"** (Verde) - El sistema WPAD está funcionando
- **"WPAD: NOT INIT"** (Rojo) - Problema con la inicialización del sistema

### 2. Valores Hexadecimales de Botones
- **"Held: 0x########"** - Valor hexadecimal de botones mantenidos presionados
- **"Pressed: 0x########"** - Valor hexadecimal de botones recién presionados

### 3. Estado Individual de Botones con Valores Esperados
- **A:0/1(0x0008)** - Botón A con valor hex esperado
- **B:0/1(0x0004)** - Botón B con valor hex esperado  
- **HOME:0/1(0x0080)** - Botón HOME con valor hex esperado
- **+:0/1(0x0010)** - Botón PLUS con valor hex esperado
- **-:0/1(0x1000)** - Botón MINUS con valor hex esperado

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
