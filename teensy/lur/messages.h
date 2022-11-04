#include <stdint.h> 

enum class MESSAGE: uint8_t {
  message_test,
  ok,
  error,
  disarm,
  arm,
  set_mode,
  get_mode,
  manual_control,
  thruster_test,
};
