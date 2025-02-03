#include <Arduino.h>
#include <Wire.h>
#include "MorseMapping.h"

#define SLEUTEL 7  // Seinsleutel van pen 7 naar GND
#define BUZZER 8   // Buzzer van pen 8 naar GND

void Decodeer(long starttijd);
void DecodeerPauze(long starttijd);
void DecodeerKar();

void setup() {
  pinMode(SLEUTEL, INPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(115200);
}

float StreepDuur = 200.0;
boolean PrevS = false;
long tStartTeken, tStartPauze;
boolean S;
String kar = "";

void loop() {
  S = !digitalRead(SLEUTEL);

  if (S) {
    if (S != PrevS) {
      tStartTeken = millis();
      DecodeerPauze(tStartPauze);
    }
    digitalWrite(BUZZER, HIGH);
  } else {
    if (S != PrevS) {
      tStartPauze = millis();
      Decodeer(tStartTeken);
    }
    digitalWrite(BUZZER, LOW);
  }

  if (abs(millis() - tStartPauze) > StreepDuur * 10) {
    DecodeerPauze(tStartPauze);
  }

  PrevS = S;
}

void Decodeer(long starttijd) {
  char teken = '?';
  long tijd = abs(millis() - starttijd);
  float PuntDuur = StreepDuur / 3.0;

  if (tijd <= 2) return;

  if (tijd <= PuntDuur) teken = '.';
  else if (tijd > StreepDuur) teken = '-';
  else if ((tijd > (StreepDuur + PuntDuur) / 1.9) && tijd <= StreepDuur) teken = '-';
  else teken = '.';

  if (teken == '-') {
    if (tijd > StreepDuur) StreepDuur++;
    if (tijd < StreepDuur) StreepDuur--;
  } else if (teken == '.') {
    if (tijd > StreepDuur / 3.0) StreepDuur++;
    if (tijd < StreepDuur / 3.0) StreepDuur--;
  }
  kar += teken;
}

void DecodeerPauze(long starttijd) {
  if (kar == "") return;
  long tijd = abs(millis() - starttijd);
  if (tijd > StreepDuur - StreepDuur / 40) {
    DecodeerKar();
  }
  if (tijd > StreepDuur * 2) {
    DecodeerKar();
    // Serial.print(" ");
    Serial.write("");
  }
}

void DecodeerKar() {
  for (int i = 0; i < sizeof(uppercaseAscii); i++) {
    if (kar == morseMapping[i]) {
    //   Serial.print(uppercaseAscii[i]);
      Serial.write(uppercaseAscii[i]);
      kar = "";
      return;
    }
  }
//   Serial.print("?");
  kar = "";
}
