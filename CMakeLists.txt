set(OpenGL_GL_PREFERENCE GLVND)
cmake_minimum_required(VERSION 3.10)
project(image-analyze VERSION 2.0 LANGUAGES C)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")

add_library(glad lib/gl.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_library(stb_image lib/stb_image.c)
target_include_directories(stb_image PUBLIC ${PROJECT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
file(GLOB SOURCES src/*.c)
add_executable(image-analyze ${SOURCES})
target_include_directories(image-analyze PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(image-analyze PRIVATE OpenGL::GL glad stb_image glfw dl m)

install(TARGETS image-analyze DESTINATION bin)
