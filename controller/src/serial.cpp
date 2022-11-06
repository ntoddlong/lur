#include "../include/serial.h"

Serial::Serial(char* p) : path(p), tty() {
  init(path, tty);
}

Serial::~Serial() {
  if (serial_port != -1)
    close(serial_port);
}

bool Serial::init(char* path, Termios& tty) {
  serial_port = open(path, O_RDWR);
  if (serial_port == -1) {
    // TODO add to log
    printf("[ERROR] Failed to open \'%s\'\n", path);
    return false;
  }
  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
    // TODO add to log
    printf("[ERROR] %i from tcgetattr: %s\n", errno, strerror(errno));
    return false;
  }
  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("[ERROR] %i from tcsetattr: %s\n", errno, strerror(errno));
      return false;
  }
  return true;
}

bool Serial::write_string(const char* s) {
  // as with read(), we may not write as many bytes as we intend, so we need to
  // track how many bytes have been written, and where to resume writing if we
  // have bytes left to write.
  size_t bytes_written = 0;
  const char *next_byte = s;
  while (bytes_written < strlen(s)+1) {
    ssize_t bytes = write(serial_port, next_byte, strlen(s)+1 - bytes_written);
    // negative bytes written indicates an error
    if (bytes < 0) {
      // TODO add this to log
      perror("write_fd::write()");
      // TODO: as with read(), if fd is a socket, then we should be checking for
      //     EINTR and continuing when the error is EINTR.
      return false;
    }
    // otherwise, advance forward to the next bytes to write
    else {
      bytes_written += bytes;
      next_byte += bytes;
    }
  }
  return true;
}

char* Serial::read_string() {
  // Allocate memory for read buffer, set size according to your needs
  char read_buf[256];

  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));

  // Read bytes. The behaviour of read() (e.g. does it block?,
  // how long does it block for?) depends on the configuration
  // settings above, specifically VMIN and VTIME
  //printf("sizeof(read_buf): %lu\nread_buf: %s\n", sizeof(read_buf), read_buf);
  int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

  // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
  if (num_bytes < 0) {
      printf("Error reading: %s\n", strerror(errno));
      return (char*)"";
  }

  if (num_bytes == 0) {
    printf("No bytes");
    return (char*)"";
  }
  // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
  // print it to the screen like this!)
  //printf("%s\n", read_buf);
  char* s = read_buf;
  return s;
}
