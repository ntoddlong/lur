#ifndef LUR_H
#define LUR_H

#pragma once

#include "Arduino.h"
#include <Servo.h>
#include "SoftwareSerial.h"
#include "ping1d.h"

const int NUM_THRUSTERS = 8;
const int MAX_THRUST = 1900;
const int MIN_THRUST = 1900;
const int thruster_pins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };
const int sonar_pins[2]    = { 0, 1 };
const int sonar_baud       = 9600;
// in milliseconds
const int sonar_timeout    = 1500;

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
  bool spin(int x, int y, int z, int roll, int pitch, int yaw);
};


/*
  Ping message types
  COMMON_ACK = 1,
  COMMON_NACK = 2,
  COMMON_ASCII_TEXT = 3,
  COMMON_GENERAL_REQUEST = 6,
  COMMON_DEVICE_INFORMATION = 4,
  COMMON_PROTOCOL_VERSION = 5,
  PING1D_SET_DEVICE_ID = 1000,
  PING1D_SET_RANGE = 1001,
  PING1D_SET_SPEED_OF_SOUND = 1002,
  PING1D_SET_MODE_AUTO = 1003,
  PING1D_SET_PING_INTERVAL = 1004,
  PING1D_SET_GAIN_SETTING = 1005,
  PING1D_SET_PING_ENABLE = 1006,
  PING1D_FIRMWARE_VERSION = 1200,
  PING1D_DEVICE_ID = 1201,
  PING1D_VOLTAGE_5 = 1202,
  PING1D_SPEED_OF_SOUND = 1203,
  PING1D_RANGE = 1204,
  PING1D_MODE_AUTO = 1205,
  PING1D_PING_INTERVAL = 1206,
  PING1D_GAIN_SETTING = 1207,
  PING1D_TRANSMIT_DURATION = 1208,
  PING1D_GENERAL_INFO = 1210,
  PING1D_DISTANCE_SIMPLE = 1211,
  PING1D_DISTANCE = 1212,
  PING1D_PROCESSOR_TEMPERATURE = 1213,
  PING1D_PCB_TEMPERATURE = 1214,
  PING1D_PING_ENABLE = 1215,
  PING1D_PROFILE = 1300,
  PING1D_GOTO_BOOTLOADER = 1100,
  PING1D_CONTINUOUS_START = 1400,
  PING1D_CONTINUOUS_STOP = 1401,
*/

/*
  https://docs.bluerobotics.com/ping-arduino/functions.html
*/
struct Sonar {
  int            baud;
  int            timeout;
  Ping1D         device;
  SoftwareSerial ping_serial;
  Sonar();
  bool init();
};

#endif
