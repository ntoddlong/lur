#include <limits.h>
#include "lur.h"

/*
 *
 ************
 *  Motors  *
 ************
 *
 */
Motors::Motors() : armed{false} { }

void Motors::init() {
  for (unsigned i = 0; i < sizeof(thrusters) / sizeof(Servo); ++i) {
    thrusters[i].attach(thruster_pins[i]);
  }
  for (unsigned i = 0; i < sizeof(thrusters) / sizeof(Servo); ++i) {
    thrusters[i].writeMicroseconds(1500);
  }
}

void Motors::arm() {
  armed = true;
}

void Motors::disarm() {
  for (unsigned i = 0; i < sizeof(thrusters) / sizeof(Servo); ++i) {
    thrusters[i].writeMicroseconds(1500);
  }
  armed = false;
}

bool Motors::set_power(const int (&values)[NUM_THRUSTERS]) {
  if (!armed) return false;
  for (unsigned i = 0; i < sizeof(values) / sizeof(int); ++i) {
    thrusters[i].writeMicroseconds(values[i]);
  }
  return true;
}

void Motors::add_to_power_vector(int (&values)[NUM_THRUSTERS], const float (&config)[NUM_THRUSTERS], int val) {
  for (unsigned i = 0; i < sizeof(values) / sizeof(int); ++i) {
    values[i] += config[i] * val;
  }
}

int Motors::normalize(int n, int min, int max) {
  int val = (n - min) * (MAX_THRUST - MIN_THRUST) / (max - min) + MIN_THRUST;
  if (val < MIN_THRUST) return MIN_THRUST;
  if (val > MAX_THRUST) return MAX_THRUST;
  return val;
}

void Motors::normalize_array(int (&values)[NUM_THRUSTERS]) {
  int min = INT_MAX, max = INT_MIN;
  for (unsigned i = 0; i < sizeof(values) / sizeof(int); ++i) {
    if (values[i] < min) {
      min = values[i];
    }
    if (values[i] > max) {
      max = values[i];
    }
  }
  for (unsigned i = 0; i < sizeof(values) / sizeof(int); ++i) {
    if (values[i] == 0) values[i] = 1500;
    else values[i] = normalize(values[i], min, max);
  }
}

bool Motors::manual_control(int x, int y, int z, int roll, int pitch, int yaw) {
  if (!armed) return false;
  int values[NUM_THRUSTERS] = { 0 };
  add_to_power_vector(values, thruster_config[0], x);
  add_to_power_vector(values, thruster_config[1], y);
  add_to_power_vector(values, thruster_config[2], z);
  add_to_power_vector(values, thruster_config[3], roll);
  add_to_power_vector(values, thruster_config[4], pitch);
  add_to_power_vector(values, thruster_config[5], yaw);
  normalize_array(values);
  return set_power(values);
}

/*
 *
 ***********
 *  SONAR  *
 ***********
 *
 */
Sonar::Sonar() : ping_serial({sonar_pins[0], sonar_pins[1]}), device{ping_serial} { }

bool Sonar::init() {
  ping_serial.begin(sonar_baud);
  while (!device.initialize() && millis() < sonar_timeout)  {
    // wait up to timeout seconds for ping1d
  }
  if (!device.update()) return false;
  return true;
}

/*
 *
 *********
 *  IMU  *
 *********
 *
 */
IMU::IMU() : device{Adafruit_BNO055(19, 0x28)} {}

bool IMU::init() {
  while (!device.begin() && millis() < imu_timeout) {
    // wait up to timeout seconds for BNO055
  }
  // not sure how to check initialization was successful
  if (millis() > imu_timeout) return false;
  return true;
}

uint8_t IMU::get_temp() {
  return device.getTemp();
}

/*
 *
 ************
 *  Jetson  *
 ************
 *
 */

Jetson::Jetson() {}

bool Jetson::init() {
  return true;
}

/*
 *
 *********
 *  Sub  *
 *********
 *
 */
Sub::Sub() : mode{Mode::disarmed}, motors(), sonar(), imu() {}

bool Sub::set_mode(Mode m) {
  switch (m) {
    case Mode::disarmed:
      motors->disarm();
      mode = m;
      break;
    case Mode::armed:
      motors->arm();
      mode = m;
      break;
    case Mode::manual:
      if (mode == Mode::disarmed) return false;
      mode = m;
      break;
    case Mode::stabilize:
      if (mode == Mode::disarmed) return false;
      mode = m;
      break;
    case Mode::depth_hold:
      if (mode == Mode::disarmed) return false;
      mode = m;
      break;
    case Mode::position_hold:
      if (mode == Mode::disarmed) return false;
      mode = m;
      break;
  }
  return true;
}
