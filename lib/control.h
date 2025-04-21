#ifndef CONTROL_H
#define CONTROL_H
#define BUZZER_A 21
#define BUZZER_B 10

#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h" // Biblioteca para gerenciamento de temporizadores e alarmes.
#include "buzina.h"
#include "ws2812.h"

#define JOYSTICK_PB 22 // GPIO para botão do Joystick
int rgb[3] = {11, 12, 13};
int volta = 55;
uint16_t wrap[5] = {60000, 40000, 14000, 24000, 33000};

// Tela 1 demonstração
void ledsControl(uint xAxis, uint yAxis)
{

    if (xAxis > 75) // joystick para direita (liga amarelo) 0
    {
        gpio_put(rgb[0], 1);
        gpio_put(rgb[2], 1);
        beep(BUZZER_A, wrap[0]);
        beep(BUZZER_B, wrap[0]);
        desenhaMatriz(samarelo);
    }
    else if (xAxis < 42) // joystick para esquerda (liga verde) 1
    {
        gpio_put(rgb[2], 0);
        gpio_put(rgb[0], 1);
        beep(BUZZER_A, wrap[1]);
        beep(BUZZER_B, wrap[1]);
        desenhaMatriz(sverde);
    }

    if (yAxis > 40) // joystick para baixo (liga azul) 2
    {
        gpio_put(rgb[2], 0);
        gpio_put(rgb[1], 1);
        beep(BUZZER_A, wrap[2]);
        beep(BUZZER_B, wrap[2]);
        desenhaMatriz(sazul);
    }
    else if (yAxis < 10) // joystick para cima (liga vermelho) 3
    {
        gpio_put(rgb[2], 1);
        gpio_put(rgb[1], 0);
        beep(BUZZER_A, wrap[3]);
        beep(BUZZER_B, wrap[3]);
        desenhaMatriz(svermelho);
    }
    if (!gpio_get(JOYSTICK_PB)) // botão do joystick (liga rosa) 4
    {
        gpio_put(rgb[2], 1);
        gpio_put(rgb[1], 1);
        beep(BUZZER_A, wrap[4]);
        beep(BUZZER_B, wrap[4]);
        desenhaMatriz(srosa);
    }
    // Joystick centro (não faz nada)
    bool desliga = xAxis > 52 && xAxis < 66 && yAxis < 31 && yAxis > 19 && gpio_get(JOYSTICK_PB);
    if (desliga)
    {
        gpio_put(rgb[2], 0);
        gpio_put(rgb[1], 0);
        gpio_put(rgb[0], 0);
        semSom();
        desenhaMatriz(cores);
    }
}
// função de escolha
int escolha(uint xAxis, uint yAxis)
{

    if (xAxis > 75) // joystick para direita (liga amarelo) 0
    {
        gpio_put(rgb[0], 1);
        gpio_put(rgb[2], 1);
        beep(BUZZER_A, wrap[0]);
        beep(BUZZER_B, wrap[0]);
        volta = 0;
    }
    else if (xAxis < 42) // joystick para esquerda (liga verde) 1
    {
        gpio_put(rgb[2], 0);
        gpio_put(rgb[0], 1);
        beep(BUZZER_A, wrap[1]);
        beep(BUZZER_B, wrap[1]);
        volta = 1;
    }

    if (yAxis > 40) // joystick para baixo (liga azul) 2
    {
        gpio_put(rgb[2], 0);
        gpio_put(rgb[1], 1);
        beep(BUZZER_A, wrap[2]);
        beep(BUZZER_B, wrap[2]);
        volta = 2;
    }
    else if (yAxis < 10) // joystick para cima (liga vermelho) 3
    {
        gpio_put(rgb[2], 1);
        gpio_put(rgb[1], 0);
        beep(BUZZER_A, wrap[3]);
        beep(BUZZER_B, wrap[3]);
        volta = 3;
    }
    if (!gpio_get(JOYSTICK_PB)) // botão do joystick (liga rosa) 4
    {
        gpio_put(rgb[2], 1);
        gpio_put(rgb[1], 1);
        beep(BUZZER_A, wrap[4]);
        beep(BUZZER_B, wrap[4]);
        volta = 4;
    }
    // Joystick centro (não faz nada)
    bool desliga = xAxis > 52 && xAxis < 66 && yAxis < 31 && yAxis > 19 && gpio_get(JOYSTICK_PB);
    if (desliga)
    {
        gpio_put(rgb[2], 0);
        gpio_put(rgb[1], 0);
        gpio_put(rgb[0], 0);
        semSom();
    }
    return volta;
}

void buzzer_led(int matrix[5][5][3], uint16_t wrap)
{
    desenhaMatriz(matrix);
    beep(BUZZER_A, wrap);
    beep(BUZZER_B, wrap);
}

#endif