#include <Wire.h>
#include <Adafruit_AHTX0.h>

#define SLAVE_ADDRESS 0x04

Adafruit_AHTX0 aht;

volatile float lastTemp = 0.0;
volatile float lastHum = 0.0;
char dataBuffer[32];

// Időzítéshez szükséges változók
unsigned long lastReadMillis = 0;    
const long readInterval = 2000;      // Szenzorolvasás 2 másodpercenként

void setup() {
  Serial.begin(9600);

  // AHT21 inicializálása
  if (!aht.begin()) {
    Serial.println("Hiba: AHT21 nem található!");
    while (1) delay(10);
  }

  // I2C Slave mód beállítása (Raspberry Pi a Master)
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData);

  Serial.println("AHT21 I2C Slave kész (Csak adatátvitel)");
}

void loop() {
  unsigned long currentMillis = millis();

  // Szenzorolvasás readInterval időközönként
  if (currentMillis - lastReadMillis >= readInterval) {
    lastReadMillis = currentMillis;

    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); 

    lastTemp = temp.temperature;
    lastHum = humidity.relative_humidity;

    // Debug adatok a Serial Monitorra
    Serial.print("Temp: "); Serial.print(lastTemp);
    Serial.print(" C | Hum: "); Serial.print(lastHum);
    Serial.println(" %");
  }
}

// Ezt hívja meg a Raspberry Pi, amikor adatot kér
void sendData() {
  char tStr[10];
  char hStr[10];
  
  // Float adatok stringgé alakítása
  dtostrf(lastTemp, 5, 2, tStr);
  dtostrf(lastHum, 5, 2, hStr);

  // Buffer ürítése és összeállítása: "24.50,55.20"
  memset(dataBuffer, 0, sizeof(dataBuffer));
  sprintf(dataBuffer, "%s,%s", tStr, hStr);

  // Küldés a Pi-nek
  Wire.write(dataBuffer);
}
