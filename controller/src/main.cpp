/*
 * -------
 * Objects
 * -------
 *  - State
 *  - Log
 *
 * ----
 * Tags
 * ----
 *  - ERROR
 *  - TODO
 *  
 * -------
 * Windows
 * -------
 *  - main_menu
 *  - window_selector
 *  - demo
 *  - style
 *  - manual_control
 *  - camera_window
 *  - app_stats
 *  - front_camera
 *  - bottom_camera
 *  - front_camera_error
 *  - bottom_camera_error
 *  - log
 *  - teensy_console
 *
 * ----------------
 * Helper Functions
 * ----------------
 *  - init_front_camera()
 *  - init_bottom_camera()
 *  - get_frame()
 */
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include <GLFW/glfw3.h>

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/opengl.hpp>

#include "../include/log.h"
#include "../include/teensy_console.h"
#include <unistd.h>

// TODO
struct State {
};


static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// ERROR
// doesn't segfault anymore but still
// have some memory errors (invalid read)
// run with valgrind
cv::Mat front_frame;
cv::Mat bottom_frame;
cv::VideoCapture front_cap;
cv::VideoCapture bottom_cap;

bool init_front_camera() {
  if (front_cap.isOpened()) {
    return true;
  }
  else if (front_cap.open(0)) {
    printf("Front camera initialized\n");
    return true;
  }
  return false;
}

bool init_bottom_camera() {
  if (bottom_cap.isOpened()) {
    return true;
  }
  else if (bottom_cap.open(1)) {
    printf("Bottom camera initialized\n");
    return true;
  }
  return false;
}

int main(int argc, char **argv) {
  static Log log((char*)"log.txt");
  static TeensyConsole teensy_console;

  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char* glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

  // Create window with graphics context
  GLFWwindow* window = glfwCreateWindow(1280, 720, "LUR controller", NULL, NULL);
  if (window == NULL)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
                                                            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  //io.Fonts->AddFontDefault();
  //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // State
  int count = 0;
  bool show_window_selector     = true;
  bool show_demo                = false;
  bool show_style               = false;
  bool show_opencv_capture      = false;
  bool show_manual_control      = false;
  bool show_camera_window       = false;
  bool show_app_stats           = true;
  bool show_front_camera        = false;
  bool show_bottom_camera       = false;
  bool show_front_camera_error  = false;
  bool show_bottom_camera_error = false;
  bool show_log                 = false;
  bool show_teensy_console      = false;
  ImVec4 clear_color            = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  //const ImVec2 p              = ImGui::GetCursorScreenPos();

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
      if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        printf("ctrl-space pressed\n");
      }
    }

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    // demo
    if (show_demo)
      ImGui::ShowDemoWindow(&show_demo);

    // main_menu
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("Windows")) {
        ImGui::MenuItem("Demo Window", "", &show_demo);
        ImGui::MenuItem("Display Settings", "", &show_style);
        ImGui::MenuItem("Manual Control", "", &show_manual_control);
        ImGui::MenuItem("Cameras", "", &show_camera_window);
        ImGui::MenuItem("Log", "", &show_log);
        ImGui::MenuItem("Teensy Console", "", &show_teensy_console);
        ImGui::MenuItem("App Stats", "", &show_app_stats);
        ImGui::Separator();
        ImGui::MenuItem("Window Selector", "", &show_window_selector);
        ImGui::EndMenu();
      }
      ImGui::Separator();
      if (ImGui::Button("DISARM")) {
        log.AddLog("[INFO] Disarm requested, asking Teensy to stop...\n");
      }
      ImGui::Separator();
      if (ImGui::Button("LOG")) {
        log.AddLog("[TEST] Test Log [%-5d:%6.1f]\n", count++, ImGui::GetTime());
      }
      ImGui::Separator();
      ImGui::EndMainMenuBar();
    }

    // window_selector
    if (show_window_selector) {
      // use this if you don't want a close button on window
      //ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Begin("Window Selector", &show_window_selector, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Checkbox("Demo Window", &show_demo);
      ImGui::Checkbox("Display Settings", &show_style);
      ImGui::Checkbox("Manual Control", &show_manual_control);
      ImGui::Checkbox("Cameras", &show_camera_window);
      ImGui::Checkbox("Log", &show_log);
      ImGui::Checkbox("Teensy Console", &show_teensy_console);
      ImGui::Checkbox("App Stats", &show_app_stats);
      //static float f = 0.0f;
      //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::End();
    }

    // log
    if (show_log) {

      // For the demo: add a debug button _BEFORE_ the normal log window contents
      // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
      // Most of the contents of the window will be added by the log.Draw() call.
      ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
      ImGui::Begin("Log", &show_log);
      //if (ImGui::SmallButton("[Debug] Add 5 entries"))
      //{
      //    static int counter = 0;
      //    const char* categories[3] = { "info", "warn", "error" };
      //    const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
      //    for (int n = 0; n < 5; n++)
      //    {
      //        const char* category = categories[counter % IM_ARRAYSIZE(categories)];
      //        const char* word = words[counter % IM_ARRAYSIZE(words)];
      //        log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
      //            ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
      //        counter++;
      //    }
      //}
      ImGui::End();

      log.Draw("Log", &show_log);
    }

    // app_stats
    if (show_app_stats) {
      ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
      ImGui::Begin("App Statistics", &show_app_stats, window_flags);
      ImGui::Text("App Statistics");
      ImGui::Separator();
      ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
      ImGui::Text("%d visible windows, %d active allocations", io.MetricsRenderWindows, io.MetricsActiveAllocations);
      ImGui::End();
    }

    // style
    if (show_style) {
      ImGui::Begin("Display Settings", &show_style, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::ShowStyleEditor();
      ImGui::Separator();
      ImGui::End();
    }

    // camera_window
    if (show_camera_window) {
      ImGui::Begin("Cameras", &show_camera_window, ImGuiWindowFlags_AlwaysAutoResize);
      if (ImGui::Button("Front Camera")) {
        if (!show_front_camera) {
          if (init_front_camera()) show_front_camera = true;
          else show_front_camera_error = true;
        }
      }
      if (front_cap.isOpened()) {
        ImGui::SameLine();
        ImGui::Text("(OPEN)");
      }
      if (ImGui::Button("Bottom Camera")) {
        if (!show_bottom_camera) {
          if (init_bottom_camera()) show_bottom_camera = true;
          else show_bottom_camera_error = true;
        }
      }
      if (bottom_cap.isOpened()) {
        ImGui::SameLine();
        ImGui::Text("(OPEN)");
      }
      ImGui::End();
    }

    // front_camera
    if (show_front_camera) {
      ImGui::Begin("Front Camera", &show_front_camera, ImGuiWindowFlags_AlwaysAutoResize);
      if (front_cap.isOpened()) {
        // ERROR
        // Address 0x19572330 is 8 bytes after a block of size 921,672 alloc'd
        front_cap.read(front_frame);
        if (front_frame.empty()) {
          log.AddLog("[ERROR] Blank frame grabbed from front_cap\n");
          show_front_camera = false;
          show_front_camera_error = true;
        }
        else {
          GLuint texture;
          glGenTextures(1, &texture);
          glBindTexture(GL_TEXTURE_2D, texture);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, front_frame.cols, front_frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, front_frame.data);

          ImGui::Image(reinterpret_cast<void*>( static_cast<intptr_t>( texture ) ), ImVec2( front_frame.cols, front_frame.rows ));
        }
      }
      else {
        show_front_camera = false;
        show_front_camera_error = true;
      }
      ImGui::End();
    }

    // bottom_camera
    if (show_bottom_camera) {
      ImGui::Begin("Bottom Camera", &show_bottom_camera, ImGuiWindowFlags_AlwaysAutoResize);
      if (bottom_cap.isOpened()) {
        bottom_cap.read(bottom_frame);
        if (bottom_frame.empty()) {
          log.AddLog("[ERROR] Blank frame grabbed from bottom_cap\n");
          show_bottom_camera = false;
          show_bottom_camera_error = true;
        }
        else {
          GLuint texture;
          glGenTextures(1, &texture);
          glBindTexture(GL_TEXTURE_2D, texture);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bottom_frame.cols, bottom_frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, bottom_frame.data);

          ImGui::Image(reinterpret_cast<void*>( static_cast<intptr_t>( texture ) ), ImVec2( bottom_frame.cols, bottom_frame.rows ));
        }
      }
      else {
        show_bottom_camera = false;
        show_bottom_camera_error = true;
      }
      ImGui::End();
    }

    // show_teensy_console
    if (show_teensy_console) {
      ImGui::Begin("Teensy Console", &show_teensy_console);
      ImGui::End();

      teensy_console.Draw("Teensy Console", &show_teensy_console);
    }

    // front_camera_error
    if (show_front_camera_error) {
      ImGui::Begin("Front Camera", &show_front_camera_error, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("ERROR\n-----\nFailed to initialize front camera");
      ImGui::End();
    }
    // bottom_camera_error
    if (show_bottom_camera_error) {
      ImGui::Begin("Bottom Camera", &show_bottom_camera_error, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("ERROR\n-----\nFailed to initialize bottom camera");
      ImGui::End();
    }

    // manual_control
    if (show_manual_control) {
      ImGui::SetNextWindowSize(ImVec2(575, 420), ImGuiCond_FirstUseEver);
      ImGui::Begin("Manual Control", &show_manual_control, ImGuiWindowFlags_NoBackground);
      ImGui::BeginChild("Controls", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
      // choose size
      static float sz = 100.0f;
      ImGui::DragFloat("Size", &sz, 0.2f, 50.0f, 100.0f, "%.0f");

      // choose color
      static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
      ImGui::ColorEdit4("Color", &colf.x);
      ImGui::PushItemWidth(-ImGui::GetFontSize()*0.25f);
      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      const ImVec2 p = ImGui::GetCursorScreenPos();
      const ImU32 col = ImColor(colf);
      const float spacing = 10.0f;
      float x = p.x + 4.0f;
      float y = p.y + 4.0f;
      float th = 1.0f;
      // forward
      x += sz + spacing;
      draw_list->AddTriangleFilled(ImVec2(x+sz*0.5f,y), ImVec2(x+sz, y+sz-0.5f), ImVec2(x, y+sz-0.5f), col);
      // left
      x -= sz + spacing;
      y += sz + spacing;
      draw_list->AddTriangleFilled(ImVec2(x,y+sz*0.5f), ImVec2(x+sz, y), ImVec2(x+sz, y+sz), col);  // Triangle
      // right
      x += 2*(sz + spacing);
      draw_list->AddTriangleFilled(ImVec2(x,y), ImVec2(x, y+sz), ImVec2(x+sz, y+sz*0.5f), col);
      // bottom
      y += sz + spacing;
      x -= sz + spacing;
      draw_list->AddTriangleFilled(ImVec2(x,y), ImVec2(x+sz, y), ImVec2(x+sz*0.5f, y+sz), col);
      // middle
      y -= sz + spacing;
      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col);
      // up
      x += 3*(sz + spacing);
      y -= sz + spacing;
      draw_list->AddTriangleFilled(ImVec2(x+sz*0.5f,y), ImVec2(x+sz, y+sz-0.5f), ImVec2(x, y+sz-0.5f), col);
      // middle
      y += sz + spacing;
      draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + sz, y + sz), col);
      // bottom
      y += sz + spacing;
      draw_list->AddTriangleFilled(ImVec2(x,y), ImVec2(x+sz, y), ImVec2(x+sz*0.5f, y+sz), col);

      ImGui::Dummy(ImVec2((sz + spacing) * 10.2f, (sz + spacing) * 3.0f));
      ImGui::PopItemWidth();
      ImGui::EndChild();
      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  front_cap.release();
  bottom_cap.release();

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
