#include <Wire.h>
#include <DHT.h> // DHT könyvtár szükséges

#define SLAVE_ADDRESS 0x04  // Arduino I2C címe
#define DHTPIN 2            // DHT11 adatlába (ide kösd a szenzort!)
#define DHTTYPE DHT11       // Szenzor típusa

DHT dht(DHTPIN, DHTTYPE);   // DHT objektum

volatile float lastTemp = 0.0;
volatile float lastHumidity = 0.0; // Nyomás helyett páratartalom
char dataBuffer[32]; 

void setup() {
  Serial.begin(9600);
  
  // 1. DHT11 inicializálása
  dht.begin();

  // 2. I2C inicializálása (Slave mód)
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData); 

  Serial.println("DHT11 -> I2C Bridge elindult...");
}

void loop() {
  // Adatok kiolvasása a szenzorból
  // A readTemperature() és readHumidity() visszatérhet NaN-nal, ha hiba van
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Ellenőrizzük, hogy sikeres volt-e az olvasás
  if (!isnan(t) && !isnan(h)) {
    lastTemp = t;
    lastHumidity = h;
  }

  // Debug kiírás
  Serial.print("Szenzor adat - Temp: ");
  Serial.print(lastTemp);
  Serial.print(" C, Paratartalom: ");
  Serial.print(lastHumidity);
  Serial.println(" %");

  delay(2000); // 2 másodperces frissítés
}

void sendData() {
  char tStr[10];
  char hStr[10];
  
  // Float konvertálás: 2 tizedesjegy
  dtostrf(lastTemp, 5, 2, tStr);
  dtostrf(lastHumidity, 5, 2, hStr);

  // Buffer nullázása és formázása
  memset(dataBuffer, 0, sizeof(dataBuffer));
  sprintf(dataBuffer, "%s,%s", tStr, hStr);

  Wire.write(dataBuffer);
}
