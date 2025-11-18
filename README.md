# Sistema de Monitoramento Cardíaco IoT com MQTT

Este repositório contém o projeto de um dispositivo IoT para monitoramento remoto de frequência cardíaca, desenvolvido como parte da disciplina de Objetos Inteligentes Conectados. O sistema coleta dados vitais, processa-os localmente e os transmite via internet para um broker MQTT.

## 📋 1. Descrição e Funcionamento
O sistema simula a leitura de um sensor de pulso, processa o sinal para calcular os Batimentos Por Minuto (BPM) e classifica o estado do paciente em quatro níveis de risco (Baixa, Normal, Atenção, Alerta).

**Fluxo de Funcionamento:**
1.  **Coleta:** O sensor analógico lê a variação de sinal simulada.
2.  **Processamento:** O ESP32 converte o sinal em BPM e determina a categoria de risco.
3.  **Feedback Local:** O display LCD exibe o BPM e a categoria; o LED RGB muda de cor e o Buzzer emite alertas sonoros.
4.  **Conectividade:** O dispositivo se conecta via WiFi e envia os dados via protocolo MQTT para a nuvem.
5.  **Comando Remoto:** O sistema também assina um tópico MQTT para receber comandos externos (ex: desligar alertas).

## 🛠️ 2. Descrição do Hardware

O projeto foi validado no simulador Wokwi com os seguintes componentes e pinagem:

| Componente | Pino ESP32 | Detalhes Técnicos |
| :--- | :---: | :--- |
| **Microcontrolador** | - | ESP32 DevKit V1 |
| **Sensor de Pulso** | GPIO 34 | Simulado por Potenciômetro (Entrada Analógica) |
| **LED RGB** | 25(R), 26(G), 27(B) | **Tipo: Ânodo Comum** (Lógica invertida no código) |
| **Display LCD** | 21(SDA), 22(SCL) | Modelo 16x2 com interface I2C (Endereço 0x27) |
| **Buzzer** | GPIO 14 | Piezoelétrico passivo |

*Nota: O LED RGB utilizado é do tipo Ânodo Comum, onde o pino comum é conectado ao 3.3V e os pinos de controle (R, G, B) são acionados com sinal LOW (ou PWM invertido).*

## 💻 3. Software e Código
O firmware (`sketch.ino`) foi desenvolvido em C++ utilizando a Arduino IDE e as seguintes bibliotecas:
* `WiFi.h`: Para conexão TCP/IP.
* `PubSubClient.h`: Para comunicação MQTT.
* `LiquidCrystal_I2C.h`: Para controle do display.

A lógica de conversão utiliza a função `map()` para transformar a leitura analógica (0-4095) em uma faixa de BPM (40-130).

## 📡 4. Interfaces e Protocolos de Comunicação

### Conectividade (TCP/IP)
O dispositivo opera sobre a pilha TCP/IP, conectando-se à rede WiFi (no simulador: "Wokwi-GUEST") para acesso à internet.

### Protocolo MQTT
O sistema utiliza o protocolo MQTT (Message Queuing Telemetry Transport) para comunicação leve e assíncrona.

* **Broker Utilizado:** `broker.emqx.io` (Público)
* **Porta:** 1883
* **QoS:** 0

### Tópicos MQTT

| Ação | Tópico | Payload (Exemplo) | Descrição |
| :--- | :--- | :--- | :--- |
| **Publicação** | `monitorcardiaco/bpm` | `85` | Envia o valor atual do BPM a cada segundo. |
| **Subscrição** | `monitorcardiaco/cmd` | `LED_RED` | Recebe comandos para alteração remota de estado. |

## 🚀 Como Reproduzir

1.  Acesse o [Wokwi](https://wokwi.com).
2.  Crie um novo projeto ESP32.
3.  Copie o conteúdo de `diagram.json` para a aba de diagrama.
4.  Copie o conteúdo de `sketch.ino` para a aba de código.
5.  Instale as bibliotecas `PubSubClient` e `LiquidCrystal I2C` no gerenciador de bibliotecas.
6.  Inicie a simulação. O monitor serial mostrará a conexão WiFi e o envio de mensagens MQTT.

---
**Autores:** Marcos Vinícius, Tainara Damascena

UNIVERSIDADE PRESBITERIANA MACKENZIE - FCI
