# CUDA Video Denoising Implementation

<div align="center">
<a href="https://isocpp.org/"><img src="https://img.shields.io/badge/Made%20with-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B" alt="Made with C++"></a>
<br>
<img src="https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat" alt="Contributions welcome">
<a href="https://github.com/yourusername/cuda-video-denoising/graphs/commit-activity"><img src="https://img.shields.io/badge/Maintained%3F-yes-green.svg" alt="Maintenance"></a>
<a href="https://opensource.org/licenses/MIT"><img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License: MIT"></a>
</div>

## 📋 Overview
This project is a parallel programming learning exercise that implements the video denoising algorithm described in:

> [Zuo, C., Liu, Y., Tan, X., Wang, W., & Zhang, M. (2013). Video Denoising Based on a Spatiotemporal Kalman-Bilateral Mixture Model. The Scientific World Journal, 2013, Article ID 438147.](https://www.hindawi.com/journals/tswj/2013/438147/) 


My primary goal is to learn parallel programming concepts by implementing a real-world research paper. This implementation:
- Translates sequential algorithms to parallel architectures
- Explores GPU memory management and optimization
- Demonstrates practical applications of parallel computing

## 🎥 Demo

https://github.com/user-attachments/assets/e67b7af1-33f8-4600-bd54-4e0348cf4ff0


## 🚀 Features
- Video denoising using OpenCv CUDA parallel processing
- Implementation of the Spatiotemporal Kalman-Bilateral Mixture Model
- Parallel computation optimizations (Not working correctly)
- Support for various video formats and resolutions

## 🛠️ Technologies
- C++17
- CUDA Toolkit
- OpenCV 4.8.0 with CUDA support
- CMake build system

## 📋 Prerequisites
- NVIDIA GPU with CUDA support
- CUDA Toolkit (11.0 or later)
- OpenCV 4.8.0 or later with CUDA support
- C++17 compatible compiler
- CMake 3.20 or later

## 🏗️ Building from Source

### Setting up CUDA
1. Install CUDA Toolkit:
```bash
# Check if CUDA is already installed
nvidia-smi

# If not installed, download and install from NVIDIA website
# https://developer.nvidia.com/cuda-downloads
```

### Installing OpenCV with CUDA (this step take time)
```bash
# Clone OpenCV repositories
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git

# Build OpenCV with CUDA support
cd opencv
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      -D WITH_CUDA=ON \
      -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
      -D CUDA_ARCH_BIN=7.5 \
      -D WITH_CUBLAS=ON \
      ..
make -j$(nproc)
sudo make install
```

### Building the Project
```bash
# Clone this repository
git clone https://github.com/yourusername/cuda-video-denoising.git
cd cuda-video-denoising

# Build
cmake .
make -j $(nproc)
```

## 💻 Usage
```bash
# Basic usage
./video-denoising-cuda input_video
```

## 📈 Performance Analysis

### Execution Time (ms/frame)

| Resolution | CPU         | GPU                   | Speedup |
|------------|-------------|-----------------------|---------|
| 480p       | 49.53ms     | 6.87ms                | 7.21x   |
| 720p       | 111.38ms    | 13.90ms               | 8.01x   |
| 1080p      | 248.89ms    | 29.93ms               | 8.31x   |
| 1440p      | 423.74ms    | 51.13ms               | 8.29x   |
| 4K         | 925.89ms    | 111.21ms              | 8.32x   |

#### Test Configuration
- GPU: NVIDIA GeForce RTX 2070 SUPER
- Video FPS: 60 
- Codec: H264

## 📚 References

### Original Research
```bibtex
@article{zuo2013video,
  title={Video Denoising Based on a Spatiotemporal Kalman-Bilateral Mixture Model},
  author={Zuo, Chenglin and Liu, Yu and Tan, Xin and Wang, Wei and Zhang, Maojun},
  journal={The Scientific World Journal},
  volume={2013},
  pages={1--10},
  year={2013},
  publisher={Hindawi Publishing Corporation},
  doi={10.1155/2013/438147}
}
```
