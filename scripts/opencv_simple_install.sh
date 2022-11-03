#!/bin/bash

set -e
set -o pipefail
set -u

if [ ! -d "install" ]; then
  mkdir -p install && pushd install
  sudo apt update && sudo apt install -y cmake g++ wget unzip

  wget -O opencv.zip https://github.com/opencv/opencv/archive/4.6.0.zip
  wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.6.0.zip
  unzip opencv.zip
  unzip opencv_contrib.zip
  mv opencv_contrib-4.6.0 opencv_contrib
  mv opencv-4.6.0 opencv
else 
  pushd install
fi

if [ ! -d "build" ]; then
  mkdir -p build && pushd build
  if ! command -v nvcc &> /dev/null; then
    cmake \
    -D CMAKE_BUILD_TYPE=RELEASE \
    -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
    -D WITH_OPENGL=ON \
    ../opencv
  else
    cmake \
    -D CMAKE_BUILD_TYPE=RELEASE \
    -D WITH_CUDA=ON \
    -D WITH_CUDNN=ON \
    -D OPENCV_DNN_CUDA=ON \
    -D ENABLE_FAST_MATH=1 \
    -D CUDA_FAST_MATH=1 \
    -D CUDA_ARCH_BIN=7.2 \
    -D WITH_CUBLAS=1 \
    -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
    -D WITH_OPENGL=ON \
    ../opencv
  fi
  cmake --build .
  popd
else
  pushd build
fi

make -j4
sudo make install
popd
popd
