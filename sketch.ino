#include <WiFi.h>
#include <PubSubClient.h>

#define LED_PIN 2

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const char* topic_k = "agu/iot/nhietdo";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) { message += (char)payload[i]; }
  
  float temp = message.toFloat();
  Serial.print("[Board 3 - LED] Nhiet do: "); Serial.println(temp);

  if (temp > 30.0) {
    digitalWrite(LED_PIN, HIGH); // Bật LED nếu t° > 30°C
    Serial.println("-> CANH BAO: Nhiet do > 30*C! BAT LED.");
  } else {
    digitalWrite(LED_PIN, LOW);  // Tắt LED
    Serial.println("-> Nhiet do binh thuong. TAT LED.");
  }
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32_Board_Led_";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      client.subscribe(topic_k);
      Serial.println("[Board 3] Da Subscribe kenh (+k)!");
    } else { delay(2000); }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) { reconnect(); }
  client.loop();
}
