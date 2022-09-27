#include <Servo.h>
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

byte led = 0;
byte t1_pin = 1;
byte l_pin = 2;
Servo thruster1;
Servo thruster2;
Servo thruster3;
Servo thruster4;
Servo thruster5;
Servo thruster6;
Servo thruster7;
Servo thruster8;
Servo lights;

void setup() {
  // put your setup code here, to run once:

  /*
   * ** setup serial
   * ** setup pins
   * ** what else?
   */
//  Serial.println("This may be sent before your PC is able to receive");
  while (!Serial && millis() < 15000) {
    // wait up to 15 seconds for Arduino Serial Monitor
  }
//  Serial.println("This line will definitely appear in the serial monitor");
// Serial.begin(9600);
 pinMode(led, OUTPUT);
//  lights.attach(l_pin);
// thruster1.attach(t1_pin);
// thruster1.writeMicroseconds(1500); // send "stop" signal to ESC.
// delay(7000); // delay to allow the ESC to recognize the stopped signal
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:

  /* ** read serial messages from jetson
   *    should we handle configuration of thruster power
   *    on jetson or on teensy? probably teensy
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
//   Serial.flush();
   int count = 0;
   char buf[256];
   while (count < 256) {
    if (Serial.available() == 0) {  // receive all 16 bytes into "buf"
      buf[count++] = Serial.read();
    }
    else break;
  }
  Serial.printf("Read %d bytes: %s\n", count, buf);

  Serial.println("hey");
  Serial.println(Serial.baud());
//  
//  while (Serial.available() == 0);
//  
//  int val = Serial.parseInt(); 
//  
//  if(val < 1100 || val > 1900)
//  {
//    Serial.println("not valid");
//  }
//  else
//  {
//    thruster1.writeMicroseconds(val); // Send signal to ESC.
//  }
//
//    thruster1.writeMicroseconds(1500);
//    delay(1000);
//  lights.writeMicroseconds(0);
//analogWrite(led, 0);
//delay(1000);
//analogWrite(led, 500);
delay(250);
     
}
