FROM nvidia/cuda:12.0.0-cudnn8-devel-ubuntu22.04
LABEL maintainer="https://github.com/NicolasQueiroga"

COPY ./code /code

WORKDIR /code

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt -y upgrade && \
    apt install -y wget git cmake gcc g++ \
    python3 python3-dev python3-numpy \
    libavcodec-dev libavformat-dev libswscale-dev \
    libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev \
    libgtk-3-dev libpng-dev libjpeg-dev libopenexr-dev libtiff-dev libwebp-dev && \
    git clone https://github.com/opencv/opencv.git /opencv && \
    git clone https://github.com/opencv/opencv_contrib.git /opencv_contrib && \
    cd /opencv && mkdir build && cd build && \
    cmake \
    -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D WITH_CUDA=ON \
    -D WITH_CUDNN=ON \
    -D WITH_CUBLAS=ON \
    -D WITH_TBB=ON \
    -D OPENCV_DNN_CUDA=ON \
    -D OPENCV_ENABLE_NONFREE=ON \
    -D CUDA_ARCH_BIN=7.5 \
    -D OPENCV_EXTRA_MODULES_PATH=/opencv_contrib/modules \
    -D BUILD_EXAMPLES=OFF \
    -D HAVE_opencv_python3=ON \
    .. && \
    make -j$(nproc) && make install && \
    cd / && rm -rf /opencv && rm -rf /opencv_contrib && \
    apt-get clean && rm -rf /var/lib/apt/lists/* && \
    rm -rf /var/cache/apt/archives/*
