
//#include <WiFi.h>
#include <WiFiClient.h>

#include <ESP8266WiFi.h>






#include <DHT.h>
double t =0;
double h =0;
//double temperatureC =0;
#define DHT11_PIN D4
#define SOIL_MOISTURE_PIN D0
#define GAS_SENSOR_PIN A0
#define RELAY_PIN D1
DHT dht(DHT11_PIN,DHT11);
int soilMoistureValue = 0;
int gasValue = 0;
int soilMoistureThreshold = 930;



const int pinD1 = D1;  // relay


bool bouton1Valeur ;

BLYNK_WRITE(V0)
{
  bouton1Valeur = param.asInt();  
}

void setup() {
  Serial.begin(115200);   // Initialisez la communication série
  Blynk.begin(auth, ssid, pass);
  pinMode(pinD1, OUTPUT);  // Configuration du pin en sortie
 // Pour la lecture analogique
  digitalRead(SOIL_MOISTURE_PIN);//naarfch digital wala analogue 
  analogRead(GAS_SENSOR_PIN);
  
  Serial.println("Setup complete");
  dht.begin();
  //sensors.begin();
  
}


void loop(){
  //sensors.requestTemperatures();
  t = dht.readTemperature();
  h = dht.readHumidity();
   soilMoistureValue = digitalRead(SOIL_MOISTURE_PIN);//naarch nhotouh analogue wala digital tw nthabtou 
  gasValue = analogRead(GAS_SENSOR_PIN);
  
  if (gasValue>285) {
    Blynk.virtualWrite(V4,"attention!");}

  // Afficher les valeurs des capteurs sur le moniteur série
  Serial.print("Humidité : ");
  Serial.print(h);
  Serial.print("%  Température : ");
  Serial.print(t);
  Serial.print("°C  Humidité du sol : ");
  Serial.print(soilMoistureValue);
  Serial.print("  Gaz : ");
  Serial.println(gasValue);
  if (bouton1Valeur)
  {
    digitalWrite(pinD1, HIGH); // Allume la relay en mettant le pin à HIGH
  }
  else {
    digitalWrite(pinD1, LOW);  
  }

  if (soilMoistureValue || bouton1Valeur) {
    digitalWrite(RELAY_PIN, HIGH);  // Activer la pompe
    Serial.println("Pompe activée");
  } else {
    digitalWrite(RELAY_PIN, LOW);   // Désactiver la pompe
    Serial.println("Pompe désactivée");
  }
  // Attendre une seconde avant de lire les capteurs à nouveau
  delay(1000);
  


  timer.run();
}