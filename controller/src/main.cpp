#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include <GLFW/glfw3.h>

#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/opengl.hpp>

#include <stdio.h>
#include <unistd.h>

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool get_frame(cv::VideoCapture cap, cv::Mat& frame, cv::ogl::Texture2D& texture, cv::ogl::Buffer& buffer, bool do_buffer) {
  if (!cap.read(frame))
      return false;

  if (do_buffer)
      buffer.copyFrom(frame, cv::ogl::Buffer::PIXEL_UNPACK_BUFFER, true);
  else
      texture.copyFrom(frame, true);

  return true;
}

cv::Mat front_frame;
cv::Mat bottom_frame;
cv::VideoCapture front_cap;
cv::VideoCapture bottom_cap;

bool init_front_camera() {
  if (front_cap.isOpened()) {
    return true;
  }
  else if (front_cap.open(0)) {
    return true;
  }
  return false;
}

bool init_bottom_camera() {
  if (bottom_cap.isOpened()) {
    return true;
  }
  else if (bottom_cap.open(1)) {
    return true;
  }
  return false;
}

int main(int argc, char **argv) {
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

                                                            // Setup Dear ImGui style
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
  bool show_window_selector = true;
  bool show_demo = false;
  bool show_style = false;
  bool show_opencv_capture = false;
  bool show_manual_control = false;
  bool show_camera_window = false;
  bool show_app_stats = true;
  bool show_front_camera = false;
  bool show_bottom_camera = false;
  bool show_front_camera_error = false;
  bool show_bottom_camera_error = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  //const ImVec2 p = ImGui::GetCursorScreenPos();

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
    if (show_demo)
      ImGui::ShowDemoWindow(&show_demo);

    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("Windows")) {
        ImGui::MenuItem("Cameras", "", &show_camera_window);
        ImGui::MenuItem("Display Settings", "", &show_style);
        ImGui::MenuItem("App Stats", "", &show_app_stats);
        ImGui::Separator();
        ImGui::MenuItem("Window Selector", "", &show_window_selector);
        ImGui::EndMenu();
      }
      ImGui::Separator();
      if (ImGui::Button("DISARM")) {
        printf("disarm\n");
      }
      ImGui::Separator();
      ImGui::EndMainMenuBar();
    }

    if (show_window_selector) {
      // use this if you don't want a close button on window
      //ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Begin("Window Selector", &show_window_selector, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Checkbox("Demo Window", &show_demo);
      ImGui::Checkbox("Display Settings", &show_style);
      ImGui::Checkbox("Manual Control", &show_manual_control);
      ImGui::Checkbox("Cameras", &show_camera_window);
      ImGui::Checkbox("App Stats", &show_app_stats);
      //static float f = 0.0f;
      //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      ImGui::End();
    }

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

    if (show_style) {
      ImGui::Begin("Display Settings", &show_style, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::ShowStyleEditor();
      ImGui::Separator();
      ImGui::End();
    }

    if (show_camera_window) {
      ImGui::Begin("Cameras", &show_camera_window, ImGuiWindowFlags_AlwaysAutoResize);
      if (ImGui::Button("Front Camera")) {
        if (!show_front_camera) {
          if (init_front_camera()) show_front_camera = true;
          else show_front_camera_error = true;
        }
      }
      if (ImGui::Button("Bottom Camera")) {
        if (!show_bottom_camera) {
          if (init_bottom_camera()) show_bottom_camera = true;
          else show_bottom_camera_error = true;
        }
      }
      ImGui::End();
    }

    if (show_front_camera) {
      ImGui::Begin("Front Camera", &show_front_camera, ImGuiWindowFlags_AlwaysAutoResize);
      if (front_cap.isOpened()) {
        front_cap.read(front_frame);
        // check if we succeeded
        if (front_frame.empty()) {
            printf("ERROR\n-----\nBlank frame grabbed\n");
            show_front_camera = false;
            show_front_camera_error = true;
        }
        else {
          GLuint texture;
          glGenTextures(1, &texture);
          glBindTexture(GL_TEXTURE_2D, texture);
          
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
          glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, front_frame.cols, front_frame.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, front_frame.data);
          ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(texture)), ImVec2(front_frame.cols, front_frame.rows));
        }
      }
      else {
        show_front_camera = false;
        show_front_camera_error = true;
      }
      ImGui::End();
    }

    if (show_bottom_camera) {
      ImGui::Begin("Bottom Camera", &show_bottom_camera, ImGuiWindowFlags_AlwaysAutoResize);
      if (bottom_cap.isOpened()) {
        bottom_cap.read(bottom_frame);
        if (bottom_frame.empty()) {
            printf("ERROR\n-----\nBlank frame grabbed\n");
            show_bottom_camera = false;
            show_bottom_camera_error = true;
        }
        else {
          GLuint texture;
          glGenTextures(1, &texture);
          glBindTexture(GL_TEXTURE_2D, texture);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
          glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bottom_frame.cols, bottom_frame.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, bottom_frame.data);
          ImGui::Image(reinterpret_cast<void*>( static_cast<intptr_t>( texture ) ), ImVec2( bottom_frame.cols, bottom_frame.rows ));
        }
      }
      else {
        show_bottom_camera = false;
        show_bottom_camera_error = true;
      }
      ImGui::End();
    }

    if (show_front_camera_error) {
      ImGui::Begin("Front Camera", &show_front_camera_error, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("ERROR\n-----\nFailed to initialize front camera");
      ImGui::End();
    }
    if (show_bottom_camera_error) {
      ImGui::Begin("Bottom Camera", &show_bottom_camera_error, ImGuiWindowFlags_AlwaysAutoResize);
      ImGui::Text("ERROR\n-----\nFailed to initialize bottom camera");
      ImGui::End();
    }

    if (show_manual_control) {
      ImGui::Begin("Manual Control", &show_manual_control, ImGuiWindowFlags_AlwaysAutoResize);
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

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
