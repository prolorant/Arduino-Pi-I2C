#include <Wire.h>
#include <Adafruit_BMP085.h>

#define SLAVE_ADDRESS 0x04     // Arduino I2C címe (ezen látja a Pi)

Adafruit_BMP085 bmp;           // BMP180 szenzor objektum

volatile float lastTemp = 0.0;
volatile float lastPressure = 0.0;
char dataBuffer[32]; 

void setup() {
  Serial.begin(9600);
  
  // 1. BMP180 inicializálása
  if (!bmp.begin()) {
    Serial.println("Hiba: BMP180 szenzor nem található!");
    while (1) {} // Megállás, ha nincs szenzor
  }

  // 2. I2C inicializálása (Slave mód a Pi felé)
  // Megjegyzés: A BMP180 a 0x77 címet használja, az Arduino pedig a 0x04-et.
  // A Wire könyvtár kezeli mindkettőt.
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData); 

  Serial.println("BMP180 -> I2C Bridge elindult...");
}

void loop() {
  // Adatok kiolvasása a szenzorból
  lastTemp = bmp.readTemperature();
  lastPressure = bmp.readPressure() / 100.0; // Pa -> hPa (vagy mbar) átváltás

  // Debug kiírás
  Serial.print("Szenzor adat - Temp: ");
  Serial.print(lastTemp);
  Serial.print(" C, Nyomas: ");
  Serial.print(lastPressure);
  Serial.println(" hPa");

  delay(2000); // 2 másodperces frissítés
}

void sendData() {
  // Ha még nincs adat
  if (lastTemp == 0.0) {
    Wire.write("0.00,0.00");
    return;
  }

  char tStr[10];
  char pStr[10];
  
  // Float konvertálás: 2 tizedesjegy
  dtostrf(lastTemp, 5, 2, tStr);
  dtostrf(lastPressure, 6, 2, pStr);

  memset(dataBuffer, 0, sizeof(dataBuffer));
  sprintf(dataBuffer, "%s,%s", tStr, pStr);

  Wire.write(dataBuffer);
}
