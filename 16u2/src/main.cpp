#include "Arduino.h"
#include "HID-Project.h"

const int pinLed = LED_BUILTIN;

void setup() {
  pinMode(pinLed, OUTPUT);
  for(int i = 4; i<6; i ++) {
    pinMode(i, INPUT_PULLUP);
  }
  
  Serial.begin(115200);
  Serial1.begin(115200);
  Keyboard.begin();
}

void loop() {
  if (!digitalRead(4)) {
    digitalWrite(pinLed, HIGH);
    Keyboard.write(KEY_SPACE);
    delay(300);
    digitalWrite(pinLed, LOW);
  }
  if (!digitalRead(5)) {
    digitalWrite(pinLed, HIGH);
    Keyboard.write(KEY_ENTER);
    delay(300);
    digitalWrite(pinLed, LOW);
  }
  if (!digitalRead(6)) {
    digitalWrite(pinLed, HIGH);
    Keyboard.write(HID_KEYBOARD_DELETE);
    delay(300);
    digitalWrite(pinLed, LOW);
  }
  
  char c = Serial1.read();
  if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || strchr(".,?'!/()&:;=+-_\"$@", c)) {
    Serial.print(F("USB: "));
    Serial.print(c);
    Keyboard.print(c);
  }
}

/* const uint8_t resetPin = IO_MCU_RESET_PIN;

void setup()
{
  // Set main MCU by default active
  digitalWrite(resetPin, HIGH);
  pinMode(resetPin, OUTPUT);

  // Start USB Serial
  Serial.begin(0);
}

void loop()
{
  // Check if the USB virtual serial wants a new baud rate
  static uint32_t baud = 0;
  if (Serial.baud() != baud) {
    baud = Serial.baud();
    Serial1.end();
    if (baud)
      Serial1.begin(baud);
  }

  // reset the main mcu if DTR goes HIGH
  if (Serial.dtr())
    digitalWrite(resetPin, LOW);
  else
    digitalWrite(resetPin, HIGH);

  // Serial -> USB

  // Check if any data has arrived on the USART serial port
  // and USB is ready for the next bytes
  int readAvailable = Serial1.available();
  if (readAvailable > 0) {

    int writeAvailable = Serial.availableForWrite();
    if (writeAvailable > 0) {

      // Write maximum one EP_SIZE to not block with Zero Length packets
      uint8_t buff[USB_EP_SIZE - 1];
      if (readAvailable > sizeof(buff))
        readAvailable = sizeof(buff);

      // Dont write more than USB can take to not block
      if (readAvailable > writeAvailable)
        readAvailable = writeAvailable;

      // Read data from the hardware serial port

      int n = Serial1.readBytes((char *)buff, readAvailable);

      // Write it to the USB port
      Serial.write(buff, n);
    }
  }


  // USB -> Serial

  // Check if any data has arrived on the USB virtual serial port
  // and USART is ready for the next bytes
  readAvailable = Serial.available();
  if (readAvailable > 0) {

    int writeAvailable = Serial1.availableForWrite();
    if (writeAvailable > 0) {

      // Write maximum one EP_SIZE to not block with Zero Length packets
      if (readAvailable > (USB_EP_SIZE - 1))
        readAvailable = (USB_EP_SIZE - 1);

      // Dont write more than USART buffer is free to not block
      if (readAvailable > writeAvailable)
        readAvailable = writeAvailable;

      // Read data from the USB serial port
      uint8_t buff[SERIAL_TX_BUFFER_SIZE];
      int n = Serial.readBytes((char *)buff, readAvailable);

      // Write it to the USART port
      Serial1.write(buff, n);
    }
  }
} */