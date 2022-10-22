#ifndef LUR_H
#define LUR_H

#pragma once

#include <Servo.h>
#include "SoftwareSerial.h"
#include "ping1d.h"
#include "ping360.h"

namespace lur {
  const int thruster_pins[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };
  const int sonar_pins[2]    = { 0, 1 };

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

  struct Motors;
  struct Sonar;
}

#endif
