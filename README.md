# Sistema de Monitoramento Cardíaco Remoto IoT (ESP32)

Este repositório contém a documentação e o código fonte do protótipo de um sistema de acompanhamento remoto de saúde cardiovascular desenvolvido como parte da disciplina de Objetos Inteligentes Conectados.

## 📋 Descrição do Projeto

O objetivo deste projeto é fornecer uma solução de baixo custo para o monitoramento contínuo da frequência cardíaca. O sistema coleta dados vitais através de sensores, processa as informações localmente em um microcontrolador ESP32 e exibe o status em tempo real através de atuadores visuais e sonoros. Além disso, o sistema está preparado para transmitir esses dados para a nuvem utilizando o protocolo MQTT, permitindo o acompanhamento remoto por profissionais de saúde.

**Funcionalidades:**
* Leitura simulada de batimentos cardíacos (BPM).
* Classificação de risco em 4 níveis (Baixa, Normal, Atenção, Alerta).
* Feedback visual via LED RGB e Display LCD.
* Feedback sonoro via Buzzer para situações críticas.
* Simulação de envio de telemetria via protocolo MQTT.

## 🛠️ Hardware Utilizado

O projeto foi desenvolvido e validado no simulador Wokwi, utilizando a seguinte lista de componentes:

| Componente | Quantidade | Especificação Técnica | Função |
| :--- | :---: | :--- | :--- |
| **Microcontrolador** | 1 | ESP32 DevKit V1 | Processamento e Conectividade Wi-Fi/Bluetooth |
| **Sensor de Pulso** | 1 | Potenciômetro (Simulação)* | Simula a entrada analógica do Pulse Sensor (0-3.3V) |
| **Display** | 1 | LCD 16x2 com Módulo I2C | Exibição local dos batimentos e status |
| **Atuador Visual** | 1 | LED RGB (Cátodo/Ânodo Comum) | Indicação colorida do estado de saúde |
| **Atuador Sonoro** | 1 | Buzzer Piezoelétrico 5V | Alerta sonoro para taquicardia/bradicardia |
| **Resistores** | 3 | 220Ω | Limitação de corrente para o LED RGB |

*\*Nota: No ambiente físico, substitui-se o potenciômetro pelo "Pulse Sensor" analógico compatível com Arduino/ESP32.*

### Pinagem e Conexões (Netlist)

| Componente | Pino Componente | Pino ESP32 | Observação |
| :--- | :--- | :--- | :--- |
| **Sensor (Potenciômetro)** | Sinal (S) | GPIO 34 | Entrada Analógica (ADC) |
| **Display LCD I2C** | SDA | GPIO 21 | Comunicação I2C |
| **Display LCD I2C** | SCL | GPIO 22 | Comunicação I2C |
| **LED RGB** | Vermelho (R) | GPIO 25 | Saída Digital / PWM |
| **LED RGB** | Verde (G) | GPIO 26 | Saída Digital / PWM |
| **LED RGB** | Azul (B) | GPIO 27 | Saída Digital / PWM |
| **Buzzer** | Positivo (+) | GPIO 14 | Saída Digital / PWM |

## 💻 Software e Lógica

O firmware foi desenvolvido em C++ utilizando a Arduino IDE.

### Lógica de Monitoramento
O sistema mapeia a leitura analógica (0-4095) para uma faixa de BPM (40-130) e aplica as seguintes regras condicionais:

1.  **< 60 BPM (Baixa):** LED Azul, Sem som.
2.  **60 - 100 BPM (Normal):** LED Verde, Sem som.
3.  **101 - 120 BPM (Atenção):** LED Amarelo, Buzzer intermitente (2000Hz).
4.  **> 120 BPM (Alerta):** LED Vermelho, Buzzer rápido (2500Hz).

## 📡 Protocolos e Comunicação

O projeto baseia-se na arquitetura TCP/IP para comunicação via Internet.

### Protocolo MQTT
Para o envio de dados à nuvem, utiliza-se o protocolo **MQTT (Message Queuing Telemetry Transport)**.
* **Arquitetura:** Publish/Subscribe.
* **Tópico de Publicação (Exemplo):** `saude/paciente_01/bpm`
* **Payload:** Valor inteiro do BPM.
* **Implementação:** No código atual (`sketch.ino`), a função de publicação MQTT é simulada através da porta Serial (`Serial.print`) para validação de lógica no Wokwi, mas o código está estruturado para receber a biblioteca `PubSubClient` para conexão real com brokers como Mosquitto ou HiveMQ.

## 🚀 Como Reproduzir (Wokwi)

1.  Acesse [Wokwi.com](https://wokwi.com).
2.  Crie um novo projeto para ESP32.
3.  Copie o conteúdo de `diagram.json` deste repositório para a aba de diagrama.
4.  Copie o conteúdo de `sketch.ino` deste repositório para a aba de código.
5.  Instale a biblioteca: `LiquidCrystal I2C` pelo gerenciador de bibliotecas do Wokwi.
6.  Inicie a simulação.

---
**Autores:**
Marcos Vinicius Vieira dos Santos Assis
Tainara Damascena Farias
*Universidade Presbiteriana Mackenzie - FCI*
