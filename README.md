# Wii Basic Homebrew Template

🎮 Plantilla base para crear aplicaciones y juegos Homebrew para la consola Nintendo Wii utilizando C++ y `devkitPro`.

Esta plantilla incluye una estructura de proyecto limpia y moderna con soporte para:
- GRRLIB
- PNGU
- PNG, JPEG, TTF
- Entrada desde Wiimote
- Un Makefile completo y portable
- Separación clara de `include/`, `source/`, `data/` y `build/`

---

## 📦 Estructura del proyecto

```
wii-basic-homebrew/
├── include/               # Archivos de encabezado (headers)
│   └── WiimoteManager.h
├── source/                # Archivos fuente C++
│   ├── main.cpp
│   └── WiimoteManager.cpp
├── build/                 # Archivos de compilación (generados)
├── data/                  # Recursos como imágenes y fuentes (opcional)
└── Makefile               # Makefile compatible con devkitPPC y GRRLIB
```


## 🧰 Requisitos

- [devkitPro](https://devkitpro.org/wiki/Getting_Started) (incluyendo `devkitPPC`)
- `GRRLIB`, `libpng`, `libjpeg`, `freetype`, `libfat`, `wiiuse`, `libogc`, entre otras (puedes instalar con pacman de devkitPro)

```bash
sudo dkp-pacman -S wii-dev
```

---

## 🛠 Compilación

```bash
make
```

El resultado será un archivo `.dol` que puedes ejecutar en tu Wii con USB Loader GX, Dolphin o cualquier otro loader.

---

## 📄 Créditos

Desarrollado por [Cristian Niño](https://github.com/cristianino)

---

## 🌀 Licencia

Este proyecto está bajo la licencia MIT. Puedes usarlo, modificarlo y distribuirlo libremente.
