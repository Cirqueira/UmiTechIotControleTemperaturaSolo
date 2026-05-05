#include <esp_now.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "Model.h"
#include "View.h"

#define PIN_RELE 18

AsyncWebServer server(80);

typedef struct struct_message {
    float temp;
    float umid;
    int solo;
} struct_message;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingBytes, int len) {
    struct_message data;
    memcpy(&data, incomingBytes, sizeof(data));
    
    statusAtual.tempAr = data.temp;
    statusAtual.umidSolo = data.solo;
    statusAtual.bombaAtiva = avaliarRega(data.solo);
    
    digitalWrite(PIN_RELE, statusAtual.bombaAtiva ? LOW : HIGH);
    adicionarAoHistorico(statusAtual);
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_RELE, OUTPUT);
    digitalWrite(PIN_RELE, HIGH);

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("UmiTech_Agro", "12345678");

    if (esp_now_init() == ESP_OK) {
        esp_now_register_recv_cb(OnDataRecv);
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    server.on("/dados", HTTP_GET, [](AsyncWebServerRequest *request){
        StaticJsonDocument<1500> doc;
        JsonObject atual = doc.createNestedObject("atual");
        atual["temp"] = statusAtual.tempAr;
        atual["solo"] = statusAtual.umidSolo;
        atual["bomba"] = statusAtual.bombaAtiva;

        JsonArray hist = doc.createNestedArray("historico");
        for(int i=0; i<10; i++) {
            if(historico[i].tempAr == 0) continue;
            JsonObject item = hist.createNestedObject();
            item["t"] = historico[i].tempAr;
            item["s"] = historico[i].umidSolo;
            item["b"] = historico[i].bombaAtiva ? "ON" : "OFF";
        }
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });

    server.begin();
    Serial.println("Servidor UmiTech Iniciado!");
}

void loop() {}