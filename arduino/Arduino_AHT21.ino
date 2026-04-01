#include <Wire.h>
#include <Adafruit_AHTX0.h>

#define SLAVE_ADDRESS 0x04

// RGB LED lábak
#define RED_PIN 9
#define GREEN_PIN 10

Adafruit_AHTX0 aht;

volatile float lastTemp = 0.0;
volatile float lastHum = 0.0;
char dataBuffer[32];

// Villogáshoz szükséges változók
unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // AHT21 inicializálása
  if (!aht.begin()) {
    Serial.println("Hiba: AHT21 nem található!");
    while (1) delay(10);
  }

  // I2C Slave mód a Raspberry Pi felé
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData);

  Serial.println("Rendszer kész: AHT21 + RGB Kontroll");
}

void loop() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp); // Adatok lekérése a szenzorból

  lastTemp = temp.temperature;
  lastHum = humidity.relative_humidity;

  // LED LOGIKA
  if (lastTemp >= 30.0) {
    // 30 FOK FELETT: Piros villogás
    digitalWrite(GREEN_PIN, LOW);
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(RED_PIN, ledState);
    }
  } 
  else {
    // 30 FOK ALATT: Fix Zöld
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
  }

  // Debug kiírás a Soros Monitorra
  Serial.print("Temp: "); Serial.print(lastTemp);
  Serial.print(" C | Para: "); Serial.print(lastHum);
  Serial.println(" %");

  delay(100); 
}

// Raspberry Pi kérésének kiszolgálása
void sendData() {
  char tStr[10];
  char hStr[10];
  
  // Átalakítás string formátumba (pl: "24.50,55.20")
  dtostrf(lastTemp, 5, 2, tStr);
  dtostrf(lastHum, 5, 2, hStr);

  memset(dataBuffer, 0, sizeof(dataBuffer));
  sprintf(dataBuffer, "%s,%s", tStr, hStr);

  Wire.write(dataBuffer);
}
