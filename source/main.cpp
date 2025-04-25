#include <fat.h>
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>

extern DISC_INTERFACE __io_wiisd;
extern DISC_INTERFACE __io_usbstorage;
extern DISC_INTERFACE __io_gcsda;
extern DISC_INTERFACE __io_gcsdb;

typedef struct {
    int x, y;
    int width, height;
} Paddle;

typedef struct {
    int x, y;
    int size;
} Ball;



int main() {
    fatInitDefault();
    // Inicialización del sistema de video y entrada
    VIDEO_Init();
    WPAD_Init();
    
    // Configurar formato de datos para Wiimote
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
    
    // Inicializar GRRLIB
    GRRLIB_Init();

    // Paletas
    Paddle player = {20, 200, 10, 60};      // Paleta izquierda
    Paddle cpu    = {600, 200, 10, 60};     // Paleta derecha

    // Pelota
    Ball ball = {320, 240, 10};             // Centro pantalla
    
    bool running = true;
    while (running) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        
        // Dibujar fondo negro
        GRRLIB_FillScreen(0x000000FF);  // RGBA: Negro opaco

        // Dibujar paletas (blanco)
        GRRLIB_Rectangle(player.x, player.y, player.width, player.height, 0xFFFFFFFF, true);
        GRRLIB_Rectangle(cpu.x, cpu.y, cpu.width, cpu.height, 0xFFFFFFFF, true);

        // Dibujar pelota (blanco)
        GRRLIB_Rectangle(ball.x, ball.y, ball.size, ball.size, 0xFFFFFFFF, true);
        
        
        // Aquí pronto dibujaremos paletas, pelota y puntuación
        
        GRRLIB_Render();  // Renderiza el frame actual
        
        // Salir con botón HOME
        if (pressed & WPAD_BUTTON_HOME) {
            running = false;
        }
    }
    
    // Cierre de recursos
    GRRLIB_Exit();
    return 0;
}