/*
 * Data: 04/18/2025
 * Por: Leonardo Romão
 *    Jogo de memorização de Cores e Som
 *
 * Este programa utiliza várias das funcionalidades da placa Bitdoglab, listadas a seguir:
 * LEDs RGB, matriz de LED 5x5, Botões, Joystick, display, comunicação USB e buzzer.
 * Utiliza os periféricos como ADC, PWM e interrupção do RP2040.
 * A finalidade do jogo é memorizar as cores e sons, correspondentes, para que o jogador faça a sequência correta de comandos para vencer.
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h" //Biblioteca do display
#include "lib/font.h"    //Fonte e figuras para o display e matrix de led
#include "lib/ws2812.h"  //Biblioteca matrix de led
#include "lib/gpios.h"   //Biblioteca matrix de led
#include "lib/control.h" //Biblioteca que contêm as funções de controle do programa
#include "lib/buzina.h"  // Biblioteca para o controle dos Buzzers

#define JOYSTICK_X_PIN 26 // GPIO para eixo X
#define JOYSTICK_Y_PIN 27 // GPIO para eixo Y
#define JOYSTICK_PB 22    // GPIO para botão do Joystick
#define Botao_A 5         // GPIO para botão A

// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define botao_B 6

ssd1306_t ssd; // Struct do display

// Variáveis utilizada para o debouncing
static volatile uint32_t passado = 0;
uint32_t tempo_atual;



const uint ledRGB[3] = {11, 12, 13}; // Red=13, Blue=12, Green=11

// Variáveis de controle do programa
uint flag_tela = 1;
uint estado = 1;

int chose;
bool umaVez = true;

int seq[3] = {0, 3, 4};
int f = 0;
// Fim das variáveis de controle

void gpio_irq_handler(uint gpio, uint32_t events);
void delay(uint32_t time); // Função para dar tempo para a interação com o usuário

int main()
{
  stdio_init_all(); // Habilita a comunicação usb com o pc

  // Inicializaçao do led RGB
  initLEDs(ledRGB);
  // Para ser utilizado o modo BOOTSEL com botão B
  initBotao(botao_B);
  gpio_set_irq_enabled_with_callback(botao_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  // Aqui termina o trecho para modo BOOTSEL com botão B

  initBotao(JOYSTICK_PB);                                                                   // Botão de escolha
  initBotao(Botao_A);                                                                       // Botão de Confirmação
  gpio_set_irq_enabled_with_callback(Botao_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // interrupção butão a
  // Inicializa a estrutura do display
  initDisplay(&ssd);

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  // Função para iniciar o PWM com a configuração pré definida
  initPwm();

  // Funções para iniciar o ADC dos Joysticks
  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN);

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  desenhaMatriz(desliga_LED); // Função para desligar todos os LEDs ws2812
  desenhaMatriz(cores);

  uint16_t adc_value_x;
  uint16_t adc_value_y;
  char str_x[5]; // Buffer para armazenar a string
  char str_y[5]; // Buffer para armazenar a string

  bool cor = true;

  uint16_t wrap[5] = {60000, 40000, 14000, 24000, 33000}; //Variável do controle do PWM

  while (true)
  {
    adc_select_input(1); // Seleciona o ADC para eixo X. O pino 26 como entrada analógica
    adc_value_x = adc_read();
    adc_select_input(0); // Seleciona o ADC para eixo Y. O pino 27 como entrada analógica
    adc_value_y = adc_read();

    // Função para conversão dos valores do do joystick para utilizar no display sdd1306
    convert_x_value(&adc_value_x); // Função para conversão dos valores do
    convert_y_value(&adc_value_y);
    // Fim da conversão

    // Se houver alguma interrupção dos botões a tela e os leds são desativados
    if (estado != flag_tela)
    {
      // Limpa o display e Matrix de LEDs.
      ssd1306_fill(&ssd, false);
      ssd1306_send_data(&ssd);
      desenhaMatriz(desliga_LED);
      estado = flag_tela;
      umaVez = true;
    };
    // Tela 1 do programa

    switch (flag_tela)
    {
    case 1:
      if (umaVez)
      {
        umaVez = false;
        printf("\tDemonstração do jogo som e cor\n\n");
        printf("Utilize o joystick e memorize as suas respectivas cores\n");
      }
      // Funções do controle da tela 1
      ledsControl(adc_value_x, adc_value_y);
      // Atualiza o conteúdo do display com animações
      ssd1306_fill(&ssd, !cor);                                     // Limpa o display
      ssd1306_rect(&ssd, 3, 35, 56, 56, cor, !cor);                 // Desenha um retângulo
      ssd1306_rect(&ssd, adc_value_y, adc_value_x, 8, 8, cor, cor); // Desenha um retângulo
      // ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor); // Desenha um retângulo
      break;
    case 2:
      f = 0;
      if (umaVez)
      {
        umaVez = false;
        printf("Adivinhe o comando certo da cor indicada na matrix de LEDs (para confirmar aperte o botão A)\n\n");
        buzzer_led(amarelo, wrap[0]);
        delay(6e2);
        semSom();
      }
      ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);  // Desenha um retângulo
      ssd1306_draw_string(&ssd, "Adivinhe", 33, 16); // Desenha uma string
      ssd1306_draw_string(&ssd, "a cor?", 40, 36);   // Desenha uma string

      chose = escolha(adc_value_x, adc_value_y);

      break;
    case 3:
      f = 1;
      if (umaVez)
      {
        printf("Parabens, a próxima rodada já vai começar\n\n");
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);   // Desenha um retângulo
        ssd1306_draw_string(&ssd, "Muito bem", 25, 26); // Desenha uma string
        ssd1306_send_data(&ssd);                        // Atualiza o display

        delay(3e3);

        printf("Adivinhe o comando certo da cor indicada na matrix de LEDs (para confirmar aperte o botão A)\n\n");
        // Limpa o display e Matrix de LEDs.
        ssd1306_fill(&ssd, false);
        ssd1306_send_data(&ssd);
      }

      ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);  // Desenha um retângulo
      ssd1306_draw_string(&ssd, "Adivinhe", 33, 16); // Desenha uma string
      ssd1306_draw_string(&ssd, "o  som?", 40, 36);  // Desenha uma string

      if (umaVez)
      {
        ssd1306_send_data(&ssd);
        umaVez = false;
        printf("Adivinhe o comando certo do som do buzzer (para confirmar aperte o botão A)\n\n");
        beep(BUZZER_A, 24000);
        beep(BUZZER_B, 24000);
        delay(6e2);
        semSom();
      }

      chose = escolha(adc_value_x, adc_value_y);

      break;
    case 4:
      f = 2;
      if (umaVez)
      {
        printf("Parabens, a próxima rodada já vai começar\n\n");
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);   // Desenha um retângulo
        ssd1306_draw_string(&ssd, "Muito bem", 25, 26); // Desenha uma string
        ssd1306_send_data(&ssd);                        // Atualiza o display

        delay(3e3);

        printf("Adivinhe o comando certo da cor indicada na matrix de LEDs (para confirmar aperte o botão A)\n\n");
        // Limpa o display e Matrix de LEDs.
        ssd1306_fill(&ssd, false);
        ssd1306_send_data(&ssd);
      }

      ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);  // Desenha um retângulo
      ssd1306_draw_string(&ssd, "Adivinhe", 33, 16); // Desenha uma string
      ssd1306_draw_string(&ssd, "a cor?", 40, 36);   // Desenha uma string

      if (umaVez)
      {
        umaVez = false;
        desenhaMatriz(rosa);
        delay(6e2);
        semSom();
      }

      chose = escolha(adc_value_x, adc_value_y);
      break;
    case 5:
      printf("\tParabens você consegui.)\n\n");
      ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);     // Desenha um retângulo
      ssd1306_draw_string(&ssd, "Parabens", 33, 16);    // Desenha uma string
      ssd1306_draw_string(&ssd, "Voce venceu", 20, 36); // Desenha uma string
      break;
    case 10:
      printf("Você errou, tente novamente.\n\n");
      ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);    // Desenha um retângulo
      ssd1306_draw_string(&ssd, "Voce Errou", 20, 26); // Desenha uma string
      ssd1306_send_data(&ssd);                         // Atualiza o display
      delay(2e3);
      flag_tela = 2;
      break;

    case 0: // Modo bootsel
      reset_usb_boot(0, 0);
      break;
    }

    ssd1306_send_data(&ssd); // Atualiza o display

    sleep_ms(8); // Delay
  }
}

void gpio_irq_handler(uint gpio, uint32_t events) // Os botões A e B mudam as telas ou avançam as fases do jogo modificando o case no loop
{
  tempo_atual = to_ms_since_boot(get_absolute_time());
  if (tempo_atual - passado > 2e2)
  {
    if (gpio == botao_B)
    {
      flag_tela--;
    }
    else if (gpio == Botao_A && flag_tela == 1)
    {
      flag_tela++;
    }
    else if (gpio == Botao_A)
    {
      if (chose == seq[f]) // {0, 3, 4};
      {
        flag_tela++;
      }
      else
      {
        flag_tela = 10;
      }
    }

    passado = tempo_atual;
  }
}

void delay(uint32_t time) // Função para dar tempo para a interação com o usuário
{
  tempo_atual = to_ms_since_boot(get_absolute_time());
  passado = tempo_atual;
  while (tempo_atual - passado < time)
  {
    tempo_atual = to_ms_since_boot(get_absolute_time());
  }
}