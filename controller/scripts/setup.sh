#!/bin/bash

wget "https://github.com/ocornut/imgui/archive/refs/tags/v1.88.zip"
wget "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip"

unzip v1.88.zip
unzip glfw-3.3.8.zip

mkdir src/imgui
mkdir include/imgui

cp \
  imgui-1.88/imgui.cpp \
  imgui-1.88/imgui_demo.cpp \
  imgui-1.88/imgui_draw.cpp \
  imgui-1.88/imgui_tables.cpp \
  imgui-1.88/imgui_widgets.cpp \
  imgui-1.88/backends/imgui_impl_glfw.cpp \
  imgui-1.88/backends/imgui_impl_opengl3.cpp \
  src/imgui

cp \
  imgui-1.88/imconfig.h \
  imgui-1.88/imgui.h \
  imgui-1.88/backends/imgui_impl_glfw.h \
  imgui-1.88/backends/imgui_impl_opengl3.h \
  imgui-1.88/backends/imgui_impl_opengl3_loader.h \
  imgui-1.88/imgui_internal.h \
  imgui-1.88/imstb_rectpack.h \
  imgui-1.88/imstb_textedit.h \
  imgui-1.88/imstb_truetype.h \
  include/imgui

rm -rf glfw-3.3.8.zip v1.88.zip imgui-1.88
