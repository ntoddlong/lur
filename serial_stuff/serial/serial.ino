#include "lur.h"

Motors motors;
Sonar sonar;

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 15000) {
    // wait up to 15 seconds for USB Serial
  }

  if (!sonar.init()) Serial.println("ERROR: Failed to initalize sonar");
  else Serial.println("Sonar initialized");
}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned int bytes_read = 0;
  char buf[256]{0};
  while (Serial.available() && bytes_read < 256) {
    buf[bytes_read] = Serial.read();
    if (buf[bytes_read] == 0b00000001) Serial.println("0b00000001");
    if (buf[bytes_read] == 0b00000010) Serial.println("0b00000010");
    if (buf[bytes_read] == 0b00000100) Serial.println("0b00000100");
    if (buf[bytes_read] == 0b00001000) Serial.println("0b00001000");
    if (buf[bytes_read] == 0b00010000) Serial.println("0b00010000");
    if (buf[bytes_read] == 0b00100000) Serial.println("0b00100000");
    if (buf[bytes_read] == 0b01000000) Serial.println("0b01000000");
    if (buf[bytes_read] == 0b10000000) Serial.println("0b10000000");
    ++bytes_read;
  }
  buf[bytes_read] = '\0';
  Serial.flush();
  if (bytes_read) Serial.printf("I'm teensy, I heard %s\n", buf);
  delay(1000);
}
