#include <limits.h>
#include "lur.h"

/*
 *
 ************
 *  Motors  *
 ************
 *
 */
Motors::Motors() : mode(Mode::disarmed) { }

void Motors::init() {
  for (int i = 0; i < NUM_THRUSTERS; ++i) {
    thrusters[i].attach(thruster_pins[i]);
  }
  for (int i = 0; i < NUM_THRUSTERS; ++i) {
    thrusters[i].writeMicroseconds(1500);
  }
}

bool Motors::set_mode(Mode m) {
  switch (m) {
    case Mode::disarmed:
      disarm();
      mode = m;
      break;
    case Mode::depth_hold:
    case Mode::position_hold:
    case Mode::manual:
    case Mode::stabilize:
      mode = m;
      break;
  }
  return true;
}

void Motors::disarm() {
  for (int i = 0; i < NUM_THRUSTERS; ++i) {
    thrusters[i].writeMicroseconds(1500);
  }
  mode = Mode::disarmed;
}

bool Motors::set_power(const int (&values)[NUM_THRUSTERS]) {
  if (mode == Mode::disarmed) return false;
  for (int i = 0; i < NUM_THRUSTERS; ++i) {
    thrusters[i].writeMicroseconds(values[i]);
  }
  return true;
}

void Motors::add_to_power_vector(int (&values)[NUM_THRUSTERS], const float (&config)[NUM_THRUSTERS], int val) {
  for (int i = 0; i < sizeof(values) / sizeof(int); ++i) {
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
  for (int i = 0; i < NUM_THRUSTERS; ++i) {
    if (values[i] < min) {
      min = values[i];
    }
    if (values[i] > max) {
      max = values[i];
    }
  }
  for (int i = 0; i < NUM_THRUSTERS; ++i) {
    if (values[i] == 0) values[i] = 1500;
    else values[i] = normalize(values[i], min, max);
  }
}

bool Motors::spin(int x, int y, int z, int roll, int pitch, int yaw) {
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
Sonar::Sonar() : ping_serial({sonar_pins[0], sonar_pins[1]}), device(ping_serial) { }

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

IMU::IMU() : device(Adafruit_BNO055(19, 0x28)) {}

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
