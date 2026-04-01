# Arduino-Pi-I2C
# I2C alapú Szenzorhálózat és Adatgyűjtő Rendszer

## Projekt bemutatása
A rendszer célja szenzoradatok gyűjtése Arduino mikrokontrollerrel, azok továbbítása I2C protokollon keresztül egy Raspberry Pi egységnek, majd az adatok feltöltése egy távoli PHP/MySQL szerverre.

## Felépítés
- **Hardware:** Arduino Uno, Raspberry Pi 4, I2C összeköttetés.
- **Backend:** PHP API, MySQL adatbázis.
- **Kommunikáció:** - Arduino -> RPi: I2C (Slave: 0x08)
  - RPi -> Szerver: HTTP POST kérés

## Telepítés
1. Töltsd fel az `arduino` mappában lévő kódot az Unora.
2. Csatlakoztasd a lábakat: SDA (A4), SCL (A5), GND.
3. Futtasd a Python szkriptet a Raspberry-n.
