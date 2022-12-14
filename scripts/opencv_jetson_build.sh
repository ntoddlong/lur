#!/bin/bash

set -e
set -o pipefail
set -u

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
