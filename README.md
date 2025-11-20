# Sistema de Monitoramento Cardíaco IoT com ESP32

Este repositório contém o código-fonte e a documentação de um protótipo de sistema de monitoramento remoto de saúde cardiovascular. O projeto utiliza a tecnologia **Internet das Coisas (IoT)** para coletar dados de frequência cardíaca, fornecer feedback visual/sonoro local e transmitir informações em tempo real para a nuvem via protocolo MQTT.

## 📋 Sobre o Projeto

As doenças cardiovasculares são uma das principais causas de mortalidade no mundo. Este projeto visa demonstrar uma solução de baixo custo e alta eficiência para o monitoramento contínuo de sinais vitais, alinhado à **ODS 3 - Saúde e Bem-Estar**.

O sistema opera capturando dados de um sensor de pulso (simulado), processando as faixas de batimento cardíaco (BPM) e enviando a telemetria para um Broker MQTT seguro.

### Principais Funcionalidades
* **Monitoramento em Tempo Real:** Leitura contínua dos batimentos cardíacos (simulação via potenciômetro para testes).
* **Feedback Local:**
    * Display LCD para exibição do BPM e status.
    * LED RGB para indicação visual de risco (cores variáveis).
    * Buzzer para alertas sonoros em faixas críticas.
* **Conectividade IoT:** Transmissão de dados via Wi-Fi usando MQTT seguro (TLS/SSL).
* **Telemetria de Performance:** Medição do tempo de resposta do hardware (microssegundos) e latência da rede (Round-Trip Time).

## 📚 Documentação Complementar

Para manter este README conciso, os detalhes técnicos foram separados em documentos específicos:

* **[INFRASTRUCTURE.md](INFRASTRUCTURE.md):** Consulte este arquivo para ver o **Diagrama de Conexões (Pinout)**, a lista completa de materiais (BOM) e as especificações da arquitetura de rede e do Broker MQTT.
* **[OPERATION.md](OPERATION.md):** Leia este documento para entender a **Lógica de Funcionamento**, a tabela de cores/sons de alerta e como interpretar os logs de telemetria de performance (latência).

## 🚀 Como Rodar no Wokwi

Como o repositório já inclui os arquivos de configuração, você pode reproduzir a simulação rapidamente seguindo estes passos:

1.  Acesse o site do [Wokwi](https://wokwi.com/) e inicie um novo projeto para **ESP32**.
2.  **Importar o Código:**
    * Abra o arquivo `sketch.ino` deste repositório, copie todo o código e cole na aba **sketch.ino** do editor do Wokwi.
3.  **Importar o Hardware:**
    * Abra o arquivo `diagram.json` deste repositório e copie seu conteúdo.
    * No Wokwi, acesse a aba `diagram.json` e substitua o conteúdo existente pelo que você copiou. Isso montará automaticamente as conexões (fios, sensores, display) conforme o esquema do projeto.
4.  **Adicionar Bibliotecas:**
    * No gerenciador de bibliotecas do Wokwi (aba *Library Manager*), adicione as dependências:
        * `LiquidCrystal I2C`
        * `PubSubClient`
5.  **Executar:**
    * Clique no botão verde de **Play** para iniciar a simulação.
    * Gire o potenciômetro para simular a variação dos batimentos cardíacos.

## 🛠 Tecnologias Utilizadas

* **Microcontrolador:** ESP32 (DevKit V1)
* **Linguagem:** C++ (Arduino Framework)
* **Protocolo de Comunicação:** MQTT (Message Queuing Telemetry Transport)
* **Broker:** EMQX Cloud (Porta 8883 - Seguro)
* **Simulador:** Wokwi (para validação de hardware e lógica)

## 👥 Autores

* **Marcos Vinicius Vieira dos Santos Assis**
* **Tainara Damascena Farias**

**Instituição:** Universidade Presbiteriana Mackenzie  
**Faculdade:** Faculdade de Computação e Informática (FCI)

---
*Este projeto foi desenvolvido como parte dos requisitos acadêmicos da disciplina de Objetos Inteligentes Conectados.*
