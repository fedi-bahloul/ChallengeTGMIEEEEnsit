#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Wire.h>
#include <OV7670.h>

// Configurer votre réseau Wi-Fi
const char* ssid = "inoubli";
const char* password = "inoubli01";

// Configuration du bot Telegram
#define BOT_TOKEN "votre_bot_token"
#define CHAT_ID "votre_chat_id"

// Définir la broche pour le capteur PIR
#define PIR_PIN D5

// Initialiser les objets nécessaires
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Initialiser la caméra OV7670
OV7670 cam;

void setup() {
  Serial.begin(115200);
  
  // Configurer la broche du capteur PIR comme entrée
  pinMode(PIR_PIN, INPUT);

  // Connexion au réseau Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au Wi-Fi...");
  }
  Serial.println("Connecté au Wi-Fi");

  // Configurer l'horloge sécurisée pour HTTPS
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  // Attendre la synchronisation du temps
  while (time(nullptr) < 8 * 3600 * 2) {
    delay(500);
  }

  // Initialiser la caméra OV7670
  if (!cam.begin()) {
    Serial.println("Échec de l'initialisation de la caméra");
    while (1);
  }
}

void loop() {
  // Vérifier si le capteur PIR détecte un mouvement
  if (digitalRead(PIR_PIN) == HIGH) {
    Serial.println("Mouvement détecté!");

    // Capturer une image avec la caméra OV7670
    if (cam.capture()) {
      Serial.println("Image capturée!");

      // Obtenir la taille de l'image capturée
      size_t imageSize = cam.getImageSize();
      
      // Allouer un buffer pour l'image
      uint8_t* imageData = new uint8_t[imageSize];
      
      // Lire l'image dans le buffer
      cam.readImage(imageData, imageSize);
      
      // Envoyer l'image sur Telegram
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Envoi de l'image à Telegram...");
        bot.sendPhotoByBinary(CHAT_ID, "image.jpg", "image/jpeg", imageData, imageSize);
      }

      // Libérer le buffer
      delete[] imageData;
    } else {
      Serial.println("Échec de la capture de l'image");
    }

    // Attendre avant de vérifier à nouveau (éviter les déclenchements multiples)
    delay(10000);  // 10 secondes
  }
}
