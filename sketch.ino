#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ----- LCD -----
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----- PINOS -----
#define PULSE_PIN 34
#define LED_R 25
#define LED_G 26
#define LED_B 27
#define BUZZER 14

int bpm = 0;

// ----- WIFI -----
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// ----- MQTT (EMQX PUBLIC) -----
const char* mqtt_server = "broker.emqx.io";
const int   mqtt_port   = 1883;

// Tópicos
const char* topic_pub = "monitorcardiaco/bpm";
const char* topic_sub = "monitorcardiaco/cmd";

WiFiClient espClient;
PubSubClient client(espClient);

// ----- CALLBACK: mensagens recebidas do broker -----
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[MQTT] Mensagem recebida em ");
  Serial.println(topic);

  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.print("[MQTT] Conteudo: ");
  Serial.println(msg);

  // Exemplos de comandos recebidos
  if (msg == "LED_RED") {
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 255);
    analogWrite(LED_B, 255);
  } else if (msg == "LED_OFF") {
    analogWrite(LED_R, 255);
    analogWrite(LED_G, 255);
    analogWrite(LED_B, 255);
  }
}

// ----- WIFI -----
void setup_wifi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ----- MQTT RECONNECT -----
void reconnect() {
  while (!client.connected()) {
    Serial.print("[MQTT] Tentando conectar... ");
    // ID do cliente pode ser qualquer string única
    if (client.connect("ESP32_WOKWI_DEMO")) {
      Serial.println("Conectado!");
      client.subscribe(topic_sub);
      Serial.print("[MQTT] Inscrito em: ");
      Serial.println(topic_sub);
    } else {
      Serial.print("Falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando de novo em 2s...");
      delay(2000);
    }
  }
}

// ----- RGB -----
void setColor(int r, int g, int b) {
  analogWrite(LED_R, 255 - r);
  analogWrite(LED_G, 255 - g);
  analogWrite(LED_B, 255 - b);
}

// ----- SETUP -----
void setup() {
  Serial.begin(115200);

  // LCD
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Monitor Cardio");
  delay(1000);
  lcd.clear();

  // GPIO
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(PULSE_PIN, INPUT);

  // WiFi
  setup_wifi();

  // MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ----- LOOP -----
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura simulada do sensor
  int raw = analogRead(PULSE_PIN);
  bpm = map(raw, 0, 4095, 40, 130);

  // Atualiza LCD
  lcd.setCursor(0, 0);
  lcd.print("BPM: ");
  lcd.print(bpm);
  lcd.print("   ");

  // Lógica de alerta
  if (bpm < 60) {
    setColor(0, 0, 255);  // Azul
    noTone(BUZZER);
    lcd.setCursor(0, 1);
    lcd.print("Baixa         ");
  } else if (bpm <= 100) {
    setColor(0, 255, 0);  // Verde
    noTone(BUZZER);
    lcd.setCursor(0, 1);
    lcd.print("Normal        ");
  } else if (bpm <= 120) {
    setColor(255, 255, 0);  // Amarelo
    tone(BUZZER, 2000, 200);
    lcd.setCursor(0, 1);
    lcd.print("Atencao       ");
  } else {
    setColor(255, 0, 0);  // Vermelho
    tone(BUZZER, 2500, 500);
    lcd.setCursor(0, 1);
    lcd.print("ALERTA!       ");
  }

  // Publica BPM no broker
  String payload = String(bpm);
  client.publish(topic_pub, payload.c_str());

  Serial.print("[MQTT] Published ");
  Serial.print(payload);
  Serial.print(" em ");
  Serial.println(topic_pub);

  delay(1000);
}


