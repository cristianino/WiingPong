# Información de Sensores del Wiimote en Debug - WiingPong

## Nueva Funcionalidad: Panel de Sensores

Se ha agregado un **panel completo de sensores** que muestra información en tiempo real de todos los sensores del Wiimote cuando el modo debug está activo.

## Ubicación del Panel

- **Posición**: Lado derecho de la pantalla
- **Activación**: Solo visible cuando debug está habilitado (PLUS+MINUS por 4 segundos)
- **Condición**: Requiere que haya datos válidos de sensores disponibles

## Información Mostrada

### 1. 🎯 **ACELERÓMETRO**
**Qué muestra:**
- Tres barras de colores representing las fuerzas en cada eje
- **Rojo**: Eje X (izquierda/derecha) 
- **Verde**: Eje Y (adelante/atrás)
- **Azul**: Eje Z (arriba/abajo)

**Interpretación:**
- Barras hacia la derecha = fuerza positiva
- Barras hacia la izquierda = fuerza negativa  
- Línea blanca central = punto de referencia (0)
- Valores escalados desde rango bruto (0-1024) centrado en 512

### 2. 🧭 **ORIENTACIÓN**
**Qué muestra:**
- **Círculo central**: Indicador de roll y pitch
- **Punto amarillo**: Dirección del roll actual
- **Barra magenta**: Nivel de pitch (inclinación vertical)
- **Barras azules laterales**: Indicador de yaw (giro)

**Interpretación:**
- Roll: Rotación del Wiimote sobre su eje longitudinal
- Pitch: Inclinación hacia arriba/abajo  
- Yaw: Giro horizontal (como girar una llave)
- Valores en grados (-180° a +180°)

### 3. ⚡ **FUERZAS G**
**Qué muestra:**
- Tres barras horizontales de colores
- **Rojo claro**: Fuerza G en X
- **Verde claro**: Fuerza G en Y  
- **Azul claro**: Fuerza G en Z
- **Círculo amarillo**: Magnitud total de fuerza

**Interpretación:**
- 1.0 = 1G (fuerza de gravedad normal)
- Útil para detectar sacudidas, golpes, movimientos bruscos
- El círculo amarillo crece con movimientos más intensos

### 4. 📡 **SENSOR IR / BARRA DE SENSORES**
**Qué muestra:**
- **Área negra**: Simulación de la pantalla del sensor IR
- **Puntos rojos**: Fuentes IR detectadas (hasta 4)
- **Cruz verde**: Posición calculada del cursor
- **Indicadores verdes**: Número de puntos IR detectados
- **Indicador rojo**: Sin señal IR

**Interpretación:**
- Funciona con la barra de sensores de la Wii
- Los puntos rojos representan las luces IR de la barra
- La cruz verde muestra dónde está "apuntando" el Wiimote
- Útil para desarrollar funciones de puntero

## Beneficios para Desarrollo

### 🔧 **Testing Avanzado**
- Verificar que todos los sensores respondan correctamente
- Calibrar rangos de movimiento para gameplay
- Detectar problemas de hardware del Wiimote

### 🎮 **Desarrollo de Funcionalidades**
- **Controles por movimiento**: Usar acelerómetro para gameplay
- **Apuntado**: Implementar controles de puntero con IR
- **Detección de gestos**: Combinar orientación + acelerómetro
- **Feedback táctil**: Responder a movimientos específicos

### 🐛 **Diagnóstico**
- **Conectividad IR**: Verificar si la barra de sensores funciona
- **Calibración**: Asegurar que los valores estén en rangos esperados
- **Interferencias**: Detectar problemas ambientales

## Datos Técnicos

### Rangos de Valores:
- **Acelerómetro**: 0-1024 (centrado en ~512)
- **Orientación**: -180° a +180° para roll/pitch/yaw  
- **G-Force**: Valores en unidades G (1.0 = gravedad terrestre)
- **IR**: Coordenadas 0-1023 (X) y 0-767 (Y)

### Frecuencia de Actualización:
- Los datos se actualizan cada frame (~60 FPS)
- Sincronizado con el loop principal del juego

## Uso Práctico

### Para Testing:
1. **Activar debug**: PLUS+MINUS por 4 segundos
2. **Observar panel de sensores** en el lado derecho
3. **Mover el Wiimote** y verificar respuestas en tiempo real
4. **Verificar cada sensor** individualmente

### Para Desarrollo Futuro:
- Usar `input.getAccelerometerData()` para movimientos
- Usar `input.getOrientationData()` para inclinación
- Usar `input.getGForceData()` para detectar golpes/sacudidas
- Usar `input.getIRData()` para funciones de puntero

## Requisitos

- **WPAD inicializado**: El InputManager debe estar funcionando
- **Debug habilitado**: Panel solo visible en modo debug
- **Formato de datos**: WPAD configurado para `WPAD_FMT_BTNS_ACC_IR`
- **Sensores activos**: Wiimote debe estar enviando datos de sensores

Esta implementación proporciona una vista completa y profesional de todos los sensores del Wiimote, facilitando tanto el testing como el desarrollo de características avanzadas de gameplay.
