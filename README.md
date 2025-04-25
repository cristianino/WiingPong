# WiingPong 🎮

**WiingPong** es un juego tipo Pong desarrollado como homebrew para la consola Nintendo Wii. Este proyecto tiene fines educativos y está construido desde cero usando C++, GRRLIB y devkitPro.

---

## 🧰 Requisitos

- [devkitPro](https://devkitpro.org/) con `devkitPPC`
- Paquete `wii-dev` con todas las librerías necesarias:
  ```bash
  sudo dkp-pacman -S wii-dev
  ```
- Emulador Dolphin o consola Wii con Homebrew Channel para ejecutar `.dol`

---

## 🚀 Compilación

Desde la terminal de devkitPro:

```bash
make
```

Esto generará un archivo `wiingpong.dol` ejecutable para la consola Wii.

---

## 🎯 Objetivo del Juego

Controla tu paleta con el Wiimote y evita que la pelota pase. Rebótala para marcar puntos. ¡Juego clásico estilo Pong, pero en tu Wii!

---

## 📁 Estructura del Proyecto

- `source/` – Código fuente en C++
- `include/` – Archivos de cabecera
- `data/` – Recursos (imágenes, fuentes, sonidos)
- `build/` – Salida de compilación
- `Makefile` – Script de construcción

---

## 👾 Estado del Desarrollo

✅ Inicializado el bucle principal  
🔲 Paletas dibujadas  
🔲 Movimiento de jugador  
🔲 Lógica de la pelota  
🔲 Puntajes y UI  
🔲 Mejora con sonido y sprites

---

## 📜 Licencia

MIT – ¡Haz lo que quieras mientras compartas mejoras!

---

## 🙌 Autor

Desarrollado con cariño por [Tu Nombre], guiado paso a paso por ChatGPT 🤖

