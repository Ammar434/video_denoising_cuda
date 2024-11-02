cmake_minimum_required(VERSION 3.16)

# Project
project(video-denoising-cuda CUDA CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CUDA)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_ARCHITECTURES 75) 

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# CUDA flags
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O3")
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -G -g")
endif()

# Create results directory
file(MAKE_DIRECTORY results)

# Sources
set(SOURCES
    main.cpp
    src/video_processor.cpp
    src/stmkb_gpu.cpp
    src/stmkb_cpu.cpp
)

# Headers
set(HEADERS
    include/video_processor.hpp
    include/add_noise.hpp
    include/stmkb_gpu.hpp
    include/stmkb_cpu.hpp
)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Main executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set properties for the target
set_target_properties(${PROJECT_NAME} PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${OpenCV_LIBS}
        
)


message(STATUS "OpenCV_VERSION: ${OpenCV_VERSION}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")