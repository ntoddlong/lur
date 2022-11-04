#include "lur.h"

Motors motors;
Sonar sonar;
IMU imuu;

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 15000) {
    // wait up to 15 seconds for USB Serial
  }

  if (!imuu.init()) Serial.println("ERROR: Failed to initalize imu");
//  else Serial.println("imu initialized");

//  if (!motors.init()) Serial.println("ERROR: Failed to initalize motors");
//  else Serial.println("Motors initialized");

//  if (!sonar.init()) Serial.println("ERROR: Failed to initalize sonar");
//  else Serial.println("Sonar initialized");
  Serial.flush();
}

void loop() {
//  Serial.println(imuu.get_temp());
  unsigned int bytes_read = 0;
  char buf[256]{0};
  while (Serial.available() && bytes_read < 256) {
    buf[bytes_read] = Serial.read();
    ++bytes_read;
  }
  buf[bytes_read] = '\0';
  if (bytes_read) Serial.printf("[%lu] %s\n", bytes_read, buf);
  Serial.flush();
  delay(1000);
}
