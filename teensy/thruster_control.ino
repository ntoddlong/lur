const float thruster_config[6][8] = {
  /*
   *      1  2  3  4  5  6  7  8
   * x
   * y
   * z
   * roll
   * pitch
   * yaw
   * 
   */
  {  1.0,  1.0, -1.0, -1.0,  0.0,  0.0,  0.0,  0.0  },
  {  1.0, -1.0,  1.0, -1.0,  0.0,  0.0,  0.0,  0.0  },
  {  0.0,  0.0,  0.0,  0.0,  1.0,  1.0,  1.0,  1.0  },
  {  0.0,  0.0,  0.0,  0.0, -1.0,  1.0, -1.0,  1.0  },
  {  0.0,  0.0,  0.0,  0.0,  1.0,  1.0, -1.0, -1.0  },
  { -1.0,  1.0,  1.0, -1.0,  0.0,  0.0,  0.0,  0.0  }
};

void setup() {
  // put your setup code here, to run once:

  /*
   * ** setup serial
   * ** setup pins
   * ** what else?
   */
  Serial.println("This may be sent before your PC is able to receive");
  while (!Serial && millis() < 15000) {
    // wait up to 15 seconds for Arduino Serial Monitor
  }
  Serial.println("This line will definitely appear in the serial monitor");
}

void loop() {
  // put your main code here, to run repeatedly:

  /* ** read serial messages from jetson
   *    should we handle configuration of thruster power
   *    on jetson or on teensy? probably jetson
   *      message format:
   *          [enum::control_type, value
   *      or 
   *          binary value
   *          (types) byte, short (16 bits), 
   * ** multiply power value for each value needed in config matrix
   * ** ensure scaling is correct
   * ** send to thruster (I think using Servo.h)
   * ** ack to serial?
   */
   int count = 0;
   char buf[11];
   while (count < 11) {
    if (Serial.available()) {  // receive all 11 bytes into "buf"
      buf[count++] = Serial.read();
    }
    else break;
  }
  Serial.printf("Read %d bytes: %s\n", count, buf);
}
