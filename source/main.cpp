#include <fat.h>
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <ogcsys.h>
#include <gccore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    int dx, dy;  // Velocity
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
    srand((unsigned)time(NULL));

    // Paletas
    Paddle player = {20, 200, 10, 60};      // Paleta izquierda
    Paddle cpu    = {600, 200, 10, 60};     // Paleta derecha

    // Pelota
    Ball ball = {320, 240, 10, 3, 2};       // Centro pantalla, velocidad inicial
    
    int playerScore = 0;
    int cpuScore = 0;
    
    bool running = true;
    while (running) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        u32 held = WPAD_ButtonsHeld(0);

        // Player movement with Wiimote buttons (+ up, - down)
        const int paddleSpeed = 5;
        if (held & WPAD_BUTTON_PLUS) {
            player.y -= paddleSpeed;
        }
        if (held & WPAD_BUTTON_MINUS) {
            player.y += paddleSpeed;
        }
        // Clamp player paddle to screen (0-420 to fit height 60)
        if (player.y < 0) player.y = 0;
        if (player.y > 420) player.y = 420;

        // Simple CPU AI: move towards ball y
        const int cpuSpeed = 3;
        if (cpu.y + cpu.height / 2 < ball.y) {
            cpu.y += cpuSpeed;
        } else if (cpu.y + cpu.height / 2 > ball.y) {
            cpu.y -= cpuSpeed;
        }
        // Clamp CPU paddle
        if (cpu.y < 0) cpu.y = 0;
        if (cpu.y > 420) cpu.y = 420;

        // Update ball position
        ball.x += ball.dx;
        ball.y += ball.dy;

        // Ball wall collisions (top/bottom)
        if (ball.y <= 0 || ball.y >= 470) {  // 480 - size
            ball.dy = -ball.dy;
        }

        // Ball paddle collisions (simple AABB)
        bool hitPlayer = (ball.x <= player.x + player.width &&
                          ball.x + ball.size >= player.x &&
                          ball.y <= player.y + player.height &&
                          ball.y + ball.size >= player.y);
        bool hitCPU = (ball.x + ball.size >= cpu.x &&
                       ball.x <= cpu.x + cpu.width &&
                       ball.y <= cpu.y + cpu.height &&
                       ball.y + ball.size >= cpu.y);

        if (hitPlayer || hitCPU) {
            ball.dx = -ball.dx;
            // Adjust dy based on hit position for angle
            int hitY = (hitPlayer ? player.y : cpu.y);
            ball.dy = (ball.y + ball.size / 2 - hitY - player.height / 2) / 10;
            if (ball.dy > 5) ball.dy = 5;
            if (ball.dy < -5) ball.dy = -5;
        }

        // Ball out of bounds (scoring) - reset to center
        if (ball.x < 0) {  // Ball passed player (left), CPU scores
            cpuScore++;
            ball.x = 320;
            ball.y = 240;
            ball.dx = 3;  // Reset direction towards player (left)
            ball.dy = (rand() % 5) - 2;  // Random slight angle
        } else if (ball.x > 640) {  // Ball passed CPU (right), player scores
            playerScore++;
            ball.x = 320;
            ball.y = 240;
            ball.dx = -3;  // Reset towards CPU (right)
            ball.dy = (rand() % 5) - 2;
        }
        
        // Dibujar fondo negro
        GRRLIB_FillScreen(0x000000FF);  // RGBA: Negro opaco

        // Dibujar paletas (blanco)
        GRRLIB_Rectangle(player.x, player.y, player.width, player.height, 0xFFFFFFFF, true);
        GRRLIB_Rectangle(cpu.x, cpu.y, cpu.width, cpu.height, 0xFFFFFFFF, true);

        // Dibujar línea central del campo
        GRRLIB_Line(320, 0, 320, 480, 0xFFFFFFFF);

        // Dibujar pelota (blanco)
        GRRLIB_Rectangle(ball.x, ball.y, ball.size, ball.size, 0xFFFFFFFF, true);

        // TODO: Dibujar puntuación con GRRLIB_LoadTTF font when available
        // GRRLIB_Printf(250, 50, font, 0xFFFFFFFF, "Player: %d", playerScore);
        // GRRLIB_Printf(350, 50, font, 0xFFFFFFFF, "CPU: %d", cpuScore);

        // Placeholder: Draw score bars (e.g., rectangles for each point)
        for (int i = 0; i < playerScore; i++) {
            GRRLIB_Rectangle(200 + i * 15, 40, 10, 20, 0x00FF00FF, true);
        }
        for (int i = 0; i < cpuScore; i++) {
            GRRLIB_Rectangle(400 + i * 15, 40, 10, 20, 0xFF0000FF, true);
        }
        
        
        // Remove old comment
        
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