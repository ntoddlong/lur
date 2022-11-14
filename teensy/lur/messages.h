#include <stdint.h> 

enum class ID: uint8_t {
  ok,
  error,
  disarm,
  arm,
  set_mode,
  get_mode,
  manual_control,
  thruster_test,
};

enum class ERROR: uint8_t {
  invalid,
  parse,
  checksum,
  disarmed,
};

struct Message {
  ID kind;
};
