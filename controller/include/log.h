#pragma once

#include "imgui.h"
#include <stdio.h>

struct Log {
  ImGuiTextBuffer buf;
  ImGuiTextFilter filter;
  ImVector<int>   line_offsets;
  bool            auto_scroll;
  bool            write_to_file;
  int             count;
  FILE*           log_file;
  char*           log_file_path;

  Log(char* path); 
  ~Log(); 
  void Clear(); 
  void AddLog(const char* fmt, ...) IM_FMTARGS(2); 
  void Draw(const char* title, bool* p_open); 
};
