cmake_minimum_required(VERSION 3.16)
project(image-analyze VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

# ---- Dependencies ----
find_package(SFML 3 REQUIRED CONFIG COMPONENTS Graphics Window System)

add_executable(image-analyze
    src/main.cpp
    src/renderer.hpp
    src/renderer.cpp
    src/fileManager.hpp
    src/fileManager.cpp
)
target_precompile_headers(image-analyze PRIVATE src/ImageAnalyzePCH.hpp)

if (MSVC)
    target_compile_options(image-analyze PRIVATE /W4 /permissive- /external:W0)
else()
    target_compile_options(image-analyze PRIVATE
        -Wall -Wextra -Wpedantic
        -Wshadow -Wconversion -Wsign-conversion
        -Wfloat-equal -Wcast-align -Wold-style-cast
        -Wnon-virtual-dtor -Wduplicated-cond -Wduplicated-branches
        -Wnull-dereference -Wmissing-declarations -Wdouble-promotion
        -Wundef -Wformat=2
    )
endif()

target_link_libraries(image-analyze PRIVATE SFML::Graphics SFML::Window SFML::System)

# ---- Runtime resources (build-tree copy so you can run from build/) ----
configure_file(${PROJECT_SOURCE_DIR}/roboto.ttf
               ${PROJECT_BINARY_DIR}/roboto.ttf
               COPYONLY)

# Allow packagers to customize where resources install. Defaults to /usr/share/image-analyze
set(IMAGE_ANALYZE_DATA_DIR
    "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
    CACHE PATH "Install-time data directory for image-analyze resources"
)

# Build-time and install-time locations provided to the program
target_compile_definitions(image-analyze PRIVATE
    APP_BUILD_DATA_DIR="${PROJECT_BINARY_DIR}"
    APP_INSTALLED_DATA_DIR="$<IF:$<BOOL:${CMAKE_INSTALL_FULL_DATADIR}>,${CMAKE_INSTALL_FULL_DATADIR},${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}>/image-analyze"
)

# ---- Install rules ----
install(TARGETS image-analyze
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}   # -> /usr/bin
)

install(FILES ${PROJECT_SOURCE_DIR}/roboto.ttf
    DESTINATION ${IMAGE_ANALYZE_DATA_DIR}         # -> /usr/share/image-analyze
)

# ---- Enable IPO/LTO in Release if available ----
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_ok OUTPUT ipo_msg)
if (ipo_ok)
    set_property(TARGET image-analyze PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()
