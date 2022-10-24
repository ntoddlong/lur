#ifndef LUR_H
#define LUR_H

#pragma once

#include "Arduino.h"

#include <Servo.h>
#include "SoftwareSerial.h"
#include "ping1d.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <stdint.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

const int NUM_THRUSTERS    = 8;
const int MAX_THRUST       = 1900;
const int MIN_THRUST       = 1100;
const int thruster_pins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

const int sonar_pins[2]    = { 0, 1 };
const int sonar_baud       = 9600;
const int sonar_timeout    = 15000; // in milliseconds

const int imu_pin          = 19;
const int imu_i2c_address  = 0x28;
const int imu_timeout      = 15000; // in milliseconds

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

enum Mode {
  disarmed,
  stabilize,
  manual,
  depth_hold,
  position_hold,
};

struct Motors {
  Mode  mode;
  Servo thrusters[NUM_THRUSTERS];
  Motors();
  void init();
  bool set_mode(Mode m);
  void disarm();
  bool set_power(const int (&values)[NUM_THRUSTERS]);
  void add_to_power_vector(int (&values)[NUM_THRUSTERS], const float (&config)[NUM_THRUSTERS], int val);
  int normalize(int n, int min, int max);
  void normalize_array(int (&values)[NUM_THRUSTERS]);
  bool spin(int x, int y, int z, int roll, int pitch, int yaw);
};

/*
  https://docs.bluerobotics.com/ping-arduino/functions.html
*/
struct Sonar {
  Ping1D         device;
  SoftwareSerial ping_serial;
  Sonar();
  bool init();
};

struct IMU {
  Adafruit_BNO055 device;
  IMU();
  bool init(); 
  uint8_t get_temp();
};

#endif
