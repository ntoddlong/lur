#include "lur.h"

namespace lur {
  typedef struct Motors {
    Mode  mode;
    Servo thrusters[8];

    Motors() : mode(Mode::disarmed) {
      for (int i = 0; i < 8; ++i) {
        thrusters[i].attach(thruster_pins[i]);
      }
      for (int i = 0; i < 8; ++i) {
        thrusters[i].writeMicroseconds(1500);
      }
    }
  };

  typedef struct Sonar {
    int            baud;
    int            timeout;
    Ping1D         ping1d;
    SoftwareSerial ping_serial;

    Sonar(int b, int to) : ping_serial({sonar_pins[0], sonar_pins[1]}), ping1d(ping_serial), baud(b), timeout(to) {
      ping_serial.begin(baud);
      while (!ping1d.initialize() && millis() < timeout * 1000) {
        // wait up to timeout seconds for ping1d
      }
    }
  };
}
