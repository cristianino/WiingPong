# WiingPong 🎮

[![Build and Release](https://github.com/cristianino/WiingPong/actions/workflows/build-and-release.yml/badge.svg)](https://github.com/cristianino/WiingPong/actions/workflows/build-and-release.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Nintendo%20Wii-blue.svg)](https://en.wikipedia.org/wiki/Wii)

**WiingPong** es un juego clásico de Pong desarrollado como homebrew para Nintendo Wii. Construido desde cero usando C++, GRRLIB y devkitPro, ofrece una experiencia nostálgica con los controles únicos del Wiimote.

![WiingPong Screenshot](docs/screenshot.png) <!-- Añade una captura cuando tengas -->

## ✨ Características

- 🎯 **Control intuitivo** con Wiimote
- 🎮 **Gameplay clásico** de Pong
- 🏆 **Sistema de puntuación**
- 📱 **Interfaz simple y limpia**
- 🔧 **Código abierto** y modificable

## 🚀 Instalación Rápida

### Para Usuarios (Wii)

1. **Descarga** la última release desde [Releases](https://github.com/cristianino/WiingPong/releases)
2. **Extrae** el archivo `.zip` o `.tar.gz`
3. **Copia** la carpeta `apps` a la raíz de tu SD
4. **Inserta** la SD en tu Wii
5. **Ejecuta** desde Homebrew Channel

### Para Desarrolladores

```bash
# Clonar el repositorio
git clone https://github.com/cristianino/WiingPong.git
cd WiingPong

# Compilar
make clean && make

# Crear paquete para SD
make package
```

## 🛠️ Desarrollo

### Requisitos Previos

- [devkitPro](https://devkitpro.org/) con `devkitPPC`
- GRRLIB (se instala automáticamente)
- Paquetes necesarios:
  ```bash
  sudo dkp-pacman -S wii-dev ppc-libpng ppc-freetype ppc-zlib ppc-bzip2
  ```

### Comandos de Build

| Comando | Descripción |
|---------|-------------|
| `make` | Compilar el proyecto |
| `make clean` | Limpiar archivos temporales |
| `make package` | Crear paquete para SD de Wii |
| `make release` | Crear archivos de release |

### Estructura del Proyecto

```
WiingPong/
├── source/              # Código fuente C++
│   ├── main.cpp        # Punto de entrada principal
│   └── WiimoteManager.cpp  # Manejo de controles
├── include/             # Headers
│   └── WiimoteManager.h
├── build/              # Archivos compilados (generado)
├── apps/               # Paquete para SD (generado)
├── release/            # Archivos de release (generado)
├── .github/workflows/  # GitHub Actions CI/CD
├── Makefile           # Configuración de build
├── LICENSE            # Licencia MIT
├── CONTRIBUTING.md    # Guía de contribución
└── README.md          # Este archivo
```

## � Cómo Jugar

1. **Apunta** el Wiimote hacia la pantalla
2. **Mueve** el Wiimote para controlar tu paleta
3. **Refleja** la pelota hacia el oponente
4. **¡Gana** siendo el primero en alcanzar la puntuación objetivo!

### Controles

| Control | Acción |
|---------|--------|
| Wiimote Movement | Mover paleta |
| A Button | Pausar/Continuar |
| Home Button | Volver al menú |

## 🏗️ Estado del Desarrollo

- ✅ **Sistema base** - Inicialización y bucle principal
- ✅ **Build automatizado** - GitHub Actions CI/CD
- ✅ **Packaging** - Generación automática de releases
- 🔄 **Controles Wiimote** - En desarrollo
- 🔄 **Lógica de juego** - En desarrollo
- 🔲 **Sistema de audio** - Planificado
- 🔲 **Menús avanzados** - Planificado
- 🔲 **Multijugador** - Planificado

## 🤝 Contribuir

¡Las contribuciones son bienvenidas! Por favor lee [CONTRIBUTING.md](CONTRIBUTING.md) para conocer el proceso.

### Formas de Contribuir

- � **Reportar bugs** en [Issues](https://github.com/cristianino/WiingPong/issues)
- 💡 **Sugerir características** nuevas
- 🔧 **Enviar Pull Requests** con mejoras
- 📖 **Mejorar documentación**
- 🎨 **Crear assets** (sprites, sonidos, etc.)

## 📋 Roadmap

### v1.1.0 - Gameplay Básico
- [ ] Implementar movimiento de paletas
- [ ] Física de la pelota
- [ ] Detección de colisiones
- [ ] Sistema de puntuación

### v1.2.0 - Mejoras Visuales
- [ ] Efectos de partículas
- [ ] Animaciones
- [ ] Mejores gráficos

### v1.3.0 - Audio y Pulido
- [ ] Efectos de sonido
- [ ] Música de fondo
- [ ] Configuraciones de juego

## 🧪 Testing

### Emulador
- **Dolphin**: Recomendado para desarrollo
- **Configuración**: Usar controles emulados de Wiimote

### Hardware Real
- **Wii con Homebrew Channel**
- **SD Card** con el paquete instalado

## 📜 Licencia

Este proyecto está bajo la Licencia MIT. Ver [LICENSE](LICENSE) para más detalles.

## � Créditos

- **Desarrollado por**: [cristianino](https://github.com/cristianino)
- **Powered by**: [devkitPro](https://devkitpro.org/)
- **Gráficos**: [GRRLIB](https://github.com/GRRLIB/GRRLIB)
- **Inspirado en**: El clásico juego Pong

## 📞 Soporte

¿Tienes problemas o preguntas?

- 🐛 **Bugs**: [Crear Issue](https://github.com/cristianino/WiingPong/issues/new)
- 💬 **Preguntas**: [Discussions](https://github.com/cristianino/WiingPong/discussions)
- 📧 **Contacto directo**: [Abrir Issue](https://github.com/cristianino/WiingPong/issues)

---

⭐ **¡Si te gusta el proyecto, considera darle una estrella!** ⭐

*Desarrollado con ❤️ para la comunidad homebrew de Nintendo Wii*

