#include <Arduino.h>
#include "libwifi.h"
#include <WiFi.h>  // Corrección en la importación
#include <Wire.h>
#include "Adafruit_SHT31.h"

#define LED 2

Adafruit_SHT31 sht31;

const char* ssid = "iPhonepedro";
const char* password = "pedro2002";
const char *host = "codelab.denkitronik.com";                                                     
const int port = 8080;


void setup() {

    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    Serial.println("Iniciando cliente HTTP...");
    conectarWifi(ssid, password);
    randomSeed(analogRead(0));
    if (!sht31.begin(0x44)) {
        Serial.println("Error: No se encontró el SHT31");
        while (1);
    }
    }

void loop() {
    WiFiClient cliente;
   
    int Temperatura = sht31.readTemperature();
    int Humedad = sht31.readHumidity();
    Serial.println("Conectando a " + String(host) + "...");

    if (!cliente.connect(host, port)) {
        Serial.println("Error de conexión al servidor");
        cliente.stop();
        delay(4000);
        return;
    }

    // Enviar solicitud HTTP correcta
    cliente.print("GET /thingname/Sensores-pedro?temperatura=" + String(Temperatura) + 
              "&humedad=" + String(Humedad) + " HTTP/1.1\r\nHost: " + String(host) + "\r\nConnection: close\r\n\r\n");


    // Tiempo de espera para la respuesta
    unsigned long tiempo = millis();
    while (cliente.available() == 0) {
        if (millis() - tiempo > 5000) {
            Serial.println("Se agotó el tiempo de espera");
            cliente.stop();
            return;
        }
    }

    // Leer la respuesta del servidor
    while (cliente.available()) {
        String linea = cliente.readStringUntil('\r');
        Serial.println(linea);
    }

    Serial.println("Finalizando conexión");
    cliente.stop();

    // Ciclo de 10 repeticiones para generar números aleatorios
    
  
    delay(4000);

}