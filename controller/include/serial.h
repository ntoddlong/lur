#pragma once

// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

typedef struct termios Termios;

struct Serial {
  int     serial_port = -1;
  char*   path;
  Termios tty;

  Serial(char* path);
  ~Serial();
  bool init(char* path, Termios& tty);
  bool write_string(const char* s);
  char* read_string();
};
