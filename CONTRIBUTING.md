# Contributing to WiingPong

¡Gracias por tu interés en contribuir a WiingPong! 🎮

## 🚀 Cómo Contribuir

### 1. Fork del Repositorio
1. Haz fork del repositorio en GitHub
2. Clona tu fork localmente:
   ```bash
   git clone https://github.com/tu-usuario/WiingPong.git
   cd WiingPong
   ```

### 2. Configuración del Entorno de Desarrollo

#### Requisitos Previos
- **devkitPro**: Necesario para compilar para Nintendo Wii
- **GRRLIB**: Librería gráfica para Wii (se instala automáticamente con el Makefile)

#### Instalación en Linux/WSL
```bash
# Instalar devkitPro
wget https://apt.devkitpro.org/install-devkitpro-pacman
chmod +x ./install-devkitpro-pacman
sudo ./install-devkitpro-pacman

# Instalar dependencias
sudo dkp-pacman -S wii-dev
sudo dkp-pacman -S ppc-libpng ppc-freetype ppc-zlib ppc-bzip2
```

#### Compilación
```bash
make clean    # Limpiar build anterior
make          # Compilar el proyecto
make package  # Crear paquete para SD de Wii
```

### 3. Proceso de Desarrollo

#### Crear una Rama
```bash
git checkout -b feature/nueva-caracteristica
# o
git checkout -b fix/descripcion-del-bug
```

#### Hacer Cambios
1. Realiza tus cambios en el código
2. Prueba la compilación: `make clean && make`
3. Prueba en emulador o Wii real si es posible

#### Commit y Push
```bash
git add .
git commit -m "feat: descripción clara de los cambios"
git push origin feature/nueva-caracteristica
```

### 4. Pull Request

1. **Abre un Pull Request** desde tu fork hacia `master`
2. **Describe claramente** qué cambios realizaste
3. **Incluye capturas** si hay cambios visuales
4. **Menciona** si probaste en hardware real

#### Template de PR
```markdown
## Descripción
Breve descripción de los cambios realizados.

## Tipo de Cambio
- [ ] Bug fix
- [ ] Nueva característica
- [ ] Cambio que rompe compatibilidad
- [ ] Actualización de documentación

## Pruebas Realizadas
- [ ] Compilación exitosa
- [ ] Prueba en emulador
- [ ] Prueba en hardware real
- [ ] Prueba de controles

## Capturas (si aplica)
Añade capturas de pantalla si hay cambios visuales.
```

## 📋 Estándares de Código

### Estilo de Código C++
- **Indentación**: 4 espacios (no tabs)
- **Llaves**: Estilo Allman
  ```cpp
  if (condition)
  {
      // código
  }
  ```
- **Nombres**: 
  - Variables: `camelCase`
  - Funciones: `PascalCase`
  - Constantes: `UPPER_CASE`

### Commits
Usa [Conventional Commits](https://www.conventionalcommits.org/):
- `feat:` nueva característica
- `fix:` corrección de bug
- `docs:` cambios en documentación
- `style:` formato, punto y coma faltante, etc.
- `refactor:` refactorización de código
- `test:` añadir o corregir pruebas

## 🎮 Ideas para Contribuir

### Características Nuevas
- **Modos de juego**: Torneos, diferentes dificultades
- **Multijugador**: Soporte para 4 jugadores
- **Efectos visuales**: Partículas, trails de la pelota
- **Audio**: Efectos de sonido, música de fondo
- **Menús**: Sistema de menús más elaborado
- **Configuración**: Opciones de juego personalizables

### Mejoras Técnicas
- **Optimización**: Mejor rendimiento
- **Refactorización**: Código más limpio y modular
- **Documentación**: Comentarios en el código
- **Testing**: Pruebas automatizadas

### Bugs Conocidos
- Revisar Issues en GitHub para bugs reportados

## 🛠️ Estructura del Proyecto

```
WiingPong/
├── source/           # Código fuente C++
│   ├── main.cpp     # Punto de entrada
│   └── WiimoteManager.cpp
├── include/          # Headers
│   └── WiimoteManager.h
├── build/           # Archivos compilados
├── apps/            # Paquete para SD (generado)
├── release/         # Paquetes de release (generado)
├── Makefile         # Configuración de compilación
└── README.md
```

## 🤝 Código de Conducta

- **Sé respetuoso** con otros contribuidores
- **Sé constructivo** en las críticas
- **Ayuda** a otros cuando puedas
- **Mantén** las discusiones enfocadas en el proyecto

## 📞 Contacto

Si tienes preguntas:
- **Issues**: Para bugs y solicitudes de características
- **Discussions**: Para preguntas generales
- **Email**: [Tu email si quieres incluirlo]

## 🎉 Reconocimientos

Todos los contribuidores serán listados en el README y en los releases.

¡Gracias por hacer WiingPong mejor! 🚀
