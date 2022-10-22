#include "lur.h"

/*
 *
 ************
 *  Motors  *
 ************
 *
 */
Motors::Motors() : mode(Mode::disarmed) { }

void Motors::initialize() {
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

// use map and constrain?
int Motors::normalize(int n, int min=0, int max=100) {
  return (MAX_THRUST - MIN_THRUST) * ((n - min) / (max - min)) + MIN_THRUST;
}

bool Motors::spin(int x, int y, int z, int roll, int pitch, int yaw) {
  int values[NUM_THRUSTERS] = { 0 };
  add_to_power_vector(values, thruster_config[0], normalize(x));
  add_to_power_vector(values, thruster_config[1], normalize(y));
  add_to_power_vector(values, thruster_config[2], normalize(z));
  add_to_power_vector(values, thruster_config[3], normalize(roll));
  add_to_power_vector(values, thruster_config[4], normalize(pitch));
  add_to_power_vector(values, thruster_config[5], normalize(yaw));
  return set_power(values);
}

/*
 *
 ***********
 *  SONAR  *
 ***********
 *
 */
Sonar::Sonar() : ping_serial({sonar_pins[0], sonar_pins[1]}), device(ping_serial), baud(sonar_baud), timeout(sonar_timeout) {
  ping_serial.begin(baud);
  while (!device.initialize() && millis() < timeout)  {
    // wait up to timeout seconds for ping1d
  }
}
