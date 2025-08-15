cmake_minimum_required(VERSION 3.16)  # 3.16+ is comfy; bump if you need newer features
project(ImageAnalyze LANGUAGES CXX)

# C++ standard (pick what you want)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# or: target_compile_features(ImageAnalyze PRIVATE cxx_std_20) after add_executable

# SFML 3: use CONFIG mode + PascalCase components/targets
find_package(SFML 3 REQUIRED CONFIG COMPONENTS Graphics Window System)

add_executable(ImageAnalyze
    src/main.cpp
    src/renderer.hpp
    src/renderer.cpp
    src/fileManager.hpp
    src/fileManager.cpp
)

# (Optional) per-target standard instead of global:
# target_compile_features(ImageAnalyze PRIVATE cxx_std_20)

# PCH
target_precompile_headers(ImageAnalyze PRIVATE src/ImageAnalyzePCH.hpp)

# Warnings — cross-platform
if (MSVC)
    target_compile_options(ImageAnalyze PRIVATE
        /W4
        /permissive-         # be strict about standard conformance
        /external:W0         # don’t spam warnings from external headers
    )
else()
    target_compile_options(ImageAnalyze PRIVATE
        -Wall -Wextra -Wpedantic
        -Wshadow -Wconversion -Wsign-conversion
        -Wfloat-equal -Wcast-align -Wold-style-cast
        -Wnon-virtual-dtor -Wduplicated-cond -Wduplicated-branches
        -Wnull-dereference -Wmissing-declarations -Wdouble-promotion
        -Wundef -Wformat=2
    )
endif()

# Link SFML 3 imported targets (PascalCase)
target_link_libraries(ImageAnalyze PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

# (Optional) nice-to-haves:
# enable IPO/LTO in Release if available
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_ok OUTPUT ipo_msg)
if (ipo_ok)
    set_property(TARGET ImageAnalyze PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

# (Optional) AddressSanitizer in Debug on GCC/Clang
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    target_compile_options(ImageAnalyze PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(ImageAnalyze PRIVATE -fsanitize=address,undefined)
endif()
