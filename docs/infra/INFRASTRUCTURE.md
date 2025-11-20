# Infraestrutura e Hardware

Este documento descreve a montagem física (esquemático), a lista de materiais e a arquitetura de rede utilizada no projeto.

## 🔌 Lista de Componentes (BOM)

| Componente | Quantidade | Descrição |
| :--- | :---: | :--- |
| **ESP32 DevKit V1** | 1 | Microcontrolador principal com Wi-Fi e Bluetooth integrados. |
| **Potenciômetro** | 1 | Utilizado para simular o *Pulse Sensor* (entrada analógica). |
| **LED RGB** | 1 | Modelo Ânodo Comum (4 pinos). |
| **Resistores** | 3 | 220Ω ou 330Ω (para proteção dos canais do LED RGB). |
| **Buzzer** | 1 | Piezoelétrico passivo/ativo (5V). |
| **Display LCD 16x2** | 1 | Com módulo adaptador I2C. |
| **Jumpers/Fios** | - | Para conexões em protoboard. |

## 🗺️ Diagrama de Conexões (Pinout)

O mapeamento dos pinos (GPIO) no ESP32 foi configurado da seguinte maneira:

| Componente | Função | Pino no ESP32 | Observação |
| :--- | :--- | :---: | :--- |
| **Sensor (Simulado)** | Entrada de Dados | `GPIO 34` | Entrada Analógica (ADC) |
| **LED RGB (Vermelho)** | Atuador Visual | `GPIO 25` | PWM via `analogWrite` |
| **LED RGB (Verde)** | Atuador Visual | `GPIO 26` | PWM via `analogWrite` |
| **LED RGB (Azul)** | Atuador Visual | `GPIO 27` | PWM via `analogWrite` |
| **Buzzer** | Atuador Sonoro | `GPIO 14` | Controle digital/tom |
| **LCD (SDA)** | Interface I2C | `GPIO 21` | Dados I2C |
| **LCD (SCL)** | Interface I2C | `GPIO 22` | Clock I2C |

> **Nota:** O LED RGB utilizado é do tipo **Ânodo Comum**. Portanto, a lógica de acionamento no código é invertida (`255 - valor`). O pino comum do LED deve ser conectado ao 3.3V.

## ☁️ Arquitetura de Rede e MQTT

O sistema utiliza uma arquitetura *Publish/Subscribe* para comunicação leve e eficiente.

### Configurações do Broker
* **Servidor:** `t3317ac0.ala.us-east-1.emqxsl.com` (EMQX Cloud)
* **Porta:** `8883` (Conexão segura via TLS/SSL)
* **Autenticação:** Usuário e Senha definidos no firmware.

### Tópicos MQTT
1.  **Publicação (`monitorcardiaco/bpm`):**
    * O ESP32 envia o valor numérico da frequência cardíaca a cada 1 segundo (aprox).
    * Payload Exemplo: `"85"`

2.  **Subscrição (`monitorcardiaco/cmd`):**
    * O ESP32 escuta este tópico para receber comandos remotos (funcionalidade preparada para expansão futura).

### Bibliotecas Necessárias
Para compilar o código, certifique-se de instalar as seguintes bibliotecas na Arduino IDE:
* `WiFi.h` (Nativa do ESP32)
* `WiFiClientSecure.h` (Para SSL/TLS)
* `PubSubClient.h` (Cliente MQTT)
* `LiquidCrystal_I2C.h` (Controle do Display)
