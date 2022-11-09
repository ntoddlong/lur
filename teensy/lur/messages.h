#include <stdint.h> 

struct Message {
};

enum class HEADER: uint8_t {
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
