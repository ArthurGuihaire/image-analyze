cmake_minimum_required(VERSION 3.16)
project(ImageAnalyze VERSION 1.0.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Standard install dirs like bin/, lib/, share/, etc.
include(GNUInstallDirs)

# SFML 3
find_package(SFML 3 REQUIRED CONFIG COMPONENTS Graphics Window System)

add_executable(ImageAnalyze
    src/main.cpp
    src/renderer.hpp
    src/renderer.cpp
    src/fileManager.hpp
    src/fileManager.cpp
)

# PCH
target_precompile_headers(ImageAnalyze PRIVATE src/ImageAnalyzePCH.hpp)

# Warnings — cross-platform
if (MSVC)
    target_compile_options(ImageAnalyze PRIVATE
        /W4
        /permissive-
        /external:W0
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

# Link SFML 3 imported targets
target_link_libraries(ImageAnalyze PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

# ----- Runtime resource during build -----
# Keep your existing behavior for in-tree runs
configure_file(${CMAKE_SOURCE_DIR}/roboto.ttf
               ${CMAKE_BINARY_DIR}/roboto.ttf
               COPYONLY)

# ----- Install rules -----
# Install the executable to bin/
install(TARGETS ImageAnalyze
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install the font
install(FILES ${CMAKE_SOURCE_DIR}/roboto.ttf
    DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})

# ----- Enable IPO/LTO in Release if available -----
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_ok OUTPUT ipo_msg)
if (ipo_ok)
    set_property(TARGET ImageAnalyze PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

# ----- Optional: 'uninstall' target -----
# Allows:  make uninstall   (from the build dir)
add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
"if(NOT EXISTS \"${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt\")\n"
"  message(FATAL_ERROR \"No install manifest\")\n"
"endif()\n"
"file(READ \"${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt\" files)\n"
"string(REPLACE \"\\n\" \";\" files \"${files}\")\n"
"foreach(file ${files})\n"
"  message(STATUS \"Removing ${file}\")\n"
"  file(REMOVE_RECURSE \"${file}\")\n"
"endforeach()\n"
)
