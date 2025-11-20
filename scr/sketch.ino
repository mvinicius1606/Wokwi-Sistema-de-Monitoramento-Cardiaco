#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----- PINOS -----
#define PULSE_PIN 34
#define LED_R 25
#define LED_G 26
#define LED_B 27
#define BUZZER 14

int bpm = 0;

// ----- WIFI (Wokwi) -----
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// ----- SUAS CREDENCIAIS MQTT (EMQX Cloud Seguro) -----
const char* mqtt_server = "seu link do MQTT server";
const int   mqtt_port   = 8883; 
const char* mqtt_user   = "seu username";
const char* mqtt_pass   = "sua password";

const char* topic_pub = "monitorcardiaco/bpm";
const char* topic_sub = "monitorcardiaco/cmd";

// ----- VAR PARA MEDIR TEMPO -----
unsigned long mqttSendTime = 0;       
bool waitingResponse = false;         

WiFiClientSecure espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_INTERVAL 1000 

void callback(char* topic, byte* payload, unsigned int length) {
  // ... Callback padrão para medir tempo de rede ...
  Serial.print("[MQTT] Mensagem recebida: ");
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  Serial.println(msg);

  if (waitingResponse) {
    unsigned long rtt = millis() - mqttSendTime;
    waitingResponse = false;
    Serial.print("[REDE] Tempo Round-Trip MQTT: ");
    Serial.print(rtt);
    Serial.println(" ms");
  }
}

void setup_wifi() {
  delay(10);
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("[MQTT] Conectando ao Broker Seguro... ");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Conectado!");
      client.subscribe(topic_sub);
    } else {
      Serial.print("Falhou rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setColor(int r, int g, int b) {
  analogWrite(LED_R, 255 - r);
  analogWrite(LED_G, 255 - g);
  analogWrite(LED_B, 255 - b);
}

void setup() {
  Serial.begin(115200);
  randomSeed(micros());

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(PULSE_PIN, INPUT);

  // Mensagem Inicial
  lcd.setCursor(0, 0);
  lcd.print("Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Cardiaco");
  delay(2000);
  lcd.clear();

  setup_wifi();
  espClient.setInsecure(); 
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop(); 

  unsigned long now = millis();
  
  if (now - lastMsg > MSG_INTERVAL) {
    lastMsg = now;
    
    int raw = analogRead(PULSE_PIN);
    bpm = map(raw, 0, 4095, 40, 130);

    lcd.setCursor(0, 0);
    lcd.print("BPM: ");
    lcd.print(bpm);
    lcd.print("   ");
    lcd.setCursor(0, 1);
    
    // --- INÍCIO DA MEDIÇÃO DO HARDWARE ---
    unsigned long inicioAtuacao = micros(); // Marca o tempo em microssegundos

    if (bpm < 60) {
      setColor(0, 0, 255);      // Azul
      noTone(BUZZER);
      lcd.print("Baixa         ");
    } 
    else if (bpm <= 100) {
      setColor(0, 255, 0);      // Verde
      noTone(BUZZER);
      lcd.print("Normal        ");
    } 
    else if (bpm <= 120) {
      setColor(255, 0, 0);      // Vermelho (Atenção)
      tone(BUZZER, 2000, 200);  
      lcd.print("Atencao       ");
    } 
    else {
      setColor(255, 255, 0);    // Amarelo (Alerta)
      tone(BUZZER, 2500, 500);  
      lcd.print("ALERTA!       ");
    }

    // --- FIM DA MEDIÇÃO DO HARDWARE ---
    unsigned long fimAtuacao = micros();
    unsigned long tempoHardware = fimAtuacao - inicioAtuacao;

    // Mostra no terminal
    Serial.print("[HARDWARE] Tempo resposta LED/Buzzer: ");
    Serial.print(tempoHardware);
    Serial.println(" us (microssegundos)");

    // Envio MQTT
    String payload = String(bpm);
    mqttSendTime = millis();
    waitingResponse = true;

    Serial.print("[MQTT] Enviando BPM: ");
    Serial.println(payload);
    client.publish(topic_pub, payload.c_str());
  }
}
