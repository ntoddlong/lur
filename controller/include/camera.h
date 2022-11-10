#pragma once

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"

#include "imgui.h"
#include <stdio.h>

struct Camera {
  cv::VideoCapture front_capture;
  cv::VideoCapture bottom_capture;
  cv::Mat front_frame;
  cv::Mat bottom_frame;

  Camera(); 
  ~Camera(); 
  bool init();
  bool init_front_camera();
  bool init_bottom_camera();
  bool read_frame(int id);
};
