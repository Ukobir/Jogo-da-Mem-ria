# 

<h1 align="center">
  <br>
    <img width="200px" src="https://github.com/Ukobir/Jogo-da-Mem-ria/blob/main/imagens/logo.png">
  <br>
  Atividade de Revisão
  <br>
</h1>

## Descrição
O jogo da memória é composto por 2 estágios. Na primeira parte (onde o programa inicia), é uma introdução ao jogo onde o usuário interage com o joystick e com os Botões A e B, que são os controles do jogo. Ao movimentar o Joystick, é possível selecionar uma cor indicada tanto pela Matriz de LEDs, quanto pelo LED RGB e neste momento um barulho é produzido pelo Buzzer correspondente a tal cor. No total são cinco cores e sons diferentes que o jogador deve memorizar para vencer. Paralelo a isto, o display cumpre a funcionalidade obrigatória exibindo um quadrado de 8x8 pixels, que se move proporcionalmente aos valores capturados pelo Joystick. E no terminal USB é utilizado para passar informações ao jogador.
Já a segunda parte é ativada ao apertar o Botão A, nela é onde o jogo começa (se for apertado o B entra no modo Bootsel). De início, o jogo repete uma das cores na Matriz de LEDs e seu barulho com o Buzzer e pede ao jogador que selecione qual a cor e som que foi demonstrado utilizando o Joystick. Feito isso, é necessário apertar o Botão A que agora passa a confirmar a sua escolha e seguir para a próxima fase. Agora ao apertar o Botão B o usuário volta a tela ou fase anterior. Neste estágio o display não mais captura o movimento do joystick mas informa o jogador em conjunto com o terminal USB.

### Pré-requisitos

1. **Git**: Certifique-se de ter o Git instalado no seu sistema. 
2. **VS Code**: Instale o Visual Studio Code, um editor de código recomendado para desenvolvimento com o Raspberry Pi Pico.
3. **Pico SDK**: Baixe e configure o SDK do Raspberry Pi Pico, conforme as instruções da documentação oficial.
4. **Simulador Wokwi**: Utilize o simulador de eletrônica Wokwi para simular o projeto, caso não tenha acesso à placa física.
5. **Conexão USB**: Certifique-se de que o Raspberry Pi Pico W esteja conectado via USB quando for testar o código.

### Passos para Execução

1. **Clonar o repositório**: Clone o repositório utilizando o comando Git no terminal:
   
   ```bash
   git clone https://github.com/Ukobir/Jogo-da-Mem-ria
   ```
2. Abrir no VS Code: Abra a pasta clonada no VS Code e no terminal do VS Code, vá até a pasta do projeto.
4. Compilar o código.

## Testes Realizados
Foi feito diversos testes para garantir a funcionamento devido da atividade. Além de que foi organizado o código conforme explicado em aula.

## Vídeo de Demonstração
[Link do Vídeo](https://drive.google.com/file/d/16VAGrl-QEkwZkmLuySFhR5sHkDaztG20/view?usp=sharing)

