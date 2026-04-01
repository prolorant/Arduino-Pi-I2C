#include <Wire.h>

#define SLAVE_ADDRESS 0x04     // I2C slave cím (Raspberry-n: i2cdetect -y 1)

// Globális változók az adatok tárolására
volatile float lastTemp = 0.0;
volatile float lastHum = 0.0;
char dataBuffer[32]; // Elég nagy a "25.50,60.10" formátumhoz

void setup() {
  Serial.begin(9600);

  // I2C inicializálása
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData); // Akkor fut le, ha a Pi adatot kér

  // Random generátor inicializálása az analóg bemenet zajából
  randomSeed(analogRead(0));

  Serial.println("I2C Szimulált Szenzor elindult...");
  Serial.println("Csak random adatokat generálok (nincs DHT kényszer).");
}

void loop() {
  // Random adatok generálása
  lastTemp = random(1800, 3201) / 100.0;  // 18.00 .. 32.00 °C
  lastHum  = random(3500, 8501) / 100.0;  // 35.00 .. 85.00 %
  
  // Debug kiírás a soros monitorra
  Serial.print("Generált adatok: ");
  Serial.print(lastTemp, 2);
  Serial.print(" C, ");
  Serial.print(lastHum, 2);
  Serial.println(" %");

  delay(2000); // 2 másodpercenként frissítjük a belső értékeket
}

// Ez a függvény fut le, amikor a Raspberry Pi kéri az adatot
void sendData() {
  // Float → string konvertálás (5 szélesség, 2 tizedes)
  char tStr[8];
  char hStr[8];
  dtostrf(lastTemp, 5, 2, tStr);
  dtostrf(lastHum, 5, 2, hStr);

  // Összefűzés: "TEMP,HUM" formátum (pl: "24.50,55.00")
  memset(dataBuffer, 0, sizeof(dataBuffer)); // Puffer ürítése
  sprintf(dataBuffer, "%s,%s", tStr, hStr);

  // Az aktuálisan tárolt random adatok elküldése az I2C buszon
  Wire.write(dataBuffer);
}
