#include "../include/camera.h"

// ERROR
// doesn't segfault anymore but still
// have some memory errors (invalid read)
// run with valgrind
Camera::Camera() : front_capture(), bottom_capture(), front_frame(), bottom_frame() {
}
Camera::~Camera() {
  front_capture.release();
  front_capture.release();
  front_frame.release();
  bottom_frame.release();
}

bool Camera::init() {
  return Camera::init_front_camera() && Camera::init_bottom_camera();
}

bool Camera::init_front_camera() {
  if (front_capture.isOpened()) {
    return true;
  }
  else if (front_capture.open(0)) {
    return true;
  }
  return false;
}

bool Camera::init_bottom_camera() {
  if (bottom_capture.isOpened()) {
    return true;
  }
  else if (bottom_capture.open(1)) {
    return true;
  }
  return false;
}

bool Camera::read_frame(int id) {
  if (id) {
    this->bottom_capture.read(bottom_frame);
    if (this->bottom_frame.empty()) return false;
  }
  else {
    this->front_capture.read(front_frame);
    if (this->front_frame.empty()) return false;
  }
  return true;
}
