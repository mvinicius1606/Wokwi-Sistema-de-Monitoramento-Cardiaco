# Funcionamento e Lógica do Sistema

Este documento detalha o algoritmo de controle, as faixas de classificação de risco e a telemetria de desempenho implementada no firmware.

## 🧠 Lógica de Monitoramento

O sistema lê o valor analógico do sensor, converte para uma escala de **40 a 130 BPM** (Batimentos Por Minuto) e classifica o estado do paciente em quatro categorias. Cada categoria aciona uma resposta específica dos atuadores.

### Tabela de Decisão

| Faixa (BPM) | Classificação | Cor do LED | Buzzer | Mensagem no LCD |
| :--- | :--- | :--- | :--- | :--- |
| **< 60** | Baixa | 🔵 **Azul** | Desligado | `Baixa` |
| **60 a 100** | Normal | 🟢 **Verde** | Desligado | `Normal` |
| **101 a 120** | Atenção | 🔴 **Vermelho** | Tom de 2000Hz | `Atencao` |
| **> 120** | **ALERTA** | 🟡 **Amarelo** | Tom Agudo (2500Hz) | `ALERTA!` |

> **Nota sobre cores:** A cor amarela é gerada pela mistura de Vermelho (255) e Verde (255) no LED RGB.

## 🔄 Fluxo de Execução (Loop Principal)

1.  **Verificação de Conexão:** O sistema verifica se está conectado ao Wi-Fi e ao Broker MQTT. Se cair, tenta reconectar automaticamente.
2.  **Leitura e Mapeamento:** Lê o valor bruto do pino 34 (0-4095) e mapeia para BPM.
3.  **Atuação (Hardware):**
    * Calcula o tempo inicial (`micros()`).
    * Aplica a lógica da tabela acima (muda cor do LED, ativa/desativa Buzzer, atualiza LCD).
    * Calcula o tempo final para determinar a latência de hardware.
4.  **Transmissão (MQTT):**
    * Publica o BPM no tópico `monitorcardiaco/bpm`.
    * Registra o timestamp de envio para cálculo de latência de rede.

## 📊 Telemetria e Performance

O código possui instrumentação para medir a eficiência do sistema. Essas informações são exibidas no **Monitor Serial** (Baud Rate 115200).

### 1. Latência de Hardware
Mede o tempo que o ESP32 leva para processar a lógica `if/else` e enviar os sinais elétricos para o LED, Buzzer e LCD.
* *Unidade:* Microssegundos (`us`).
* *Log:* `[HARDWARE] Tempo resposta LED/Buzzer: X us`

### 2. Latência de Rede (Round-Trip Time)
Mede o tempo total para a mensagem ir até o broker e o callback de confirmação ser processado (ou uma mensagem de eco ser recebida).
* *Unidade:* Milissegundos (`ms`).
* *Log:* `[REDE] Tempo Round-Trip MQTT: Y ms`

## 🧪 Como Testar (Simulação)

1.  Carregue o código no simulador Wokwi ou no ESP32 físico.
2.  Gire o potenciômetro conectado ao **GPIO 34**.
3.  Observe:
    * O valor de BPM mudando no LCD.
    * A cor do LED alterando conforme a faixa de batimentos.
    * O som do Buzzer ativando acima de 100 BPM.
    * As mensagens `[MQTT] Enviando BPM: ...` no Monitor Serial.
