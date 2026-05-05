#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>

// --- CONFIGURAÇÕES ---
#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_PIN 34
// SUBSTITUA PELO MAC ADDRESS DO SEU RECEPTOR:
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 

DHT dht(DHTPIN, DHTTYPE);

typedef struct struct_message {
    float temp;
    float umid;
    int solo;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) return;

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) return;
}

void loop() {
  myData.temp = dht.readTemperature();
  myData.umid = dht.readHumidity();
  // Calibração: Lê 0-4095 e envia o valor bruto para o mestre tratar
  myData.solo = analogRead(SOIL_PIN); 

  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  Serial.printf("Enviado: T:%.1f S:%d\n", myData.temp, myData.solo);
  delay(5000); // Envia a cada 5 segundos
}