#pragma ONCE

#include "imgui.h"

// Portable helpers
static int   Stricmp(const char* s1, const char* s2); 
static int   Strnicmp(const char* s1, const char* s2, int n); 
static char* Strdup(const char* s); 
static void  Strtrim(char* s); 

// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
static int TextEditCallbackStub(ImGuiInputTextCallbackData* data); 

struct TeensyConsole {
  char                  input_buf[256];
  ImVector<char*>       items;
  ImVector<const char*> commands;
  ImVector<char*>       history;
  int                   history_pos;    // -1: new line, 0..history.Size-1 browsing history.
  ImGuiTextFilter       filter;
  bool                  auto_scroll;
  bool                  scroll_to_bottom;

  TeensyConsole(); 
  ~TeensyConsole(); 


  void ClearLog(); 
  void AddLog(const char* fmt, ...) IM_FMTARGS(2); 
  void Draw(const char* title, bool* p_open); 
  void ExecCommand(const char* command_line); 
  int TextEditCallback(ImGuiInputTextCallbackData* data); 
};
