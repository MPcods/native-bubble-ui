cmake_minimum_required(VERSION 3.10.2)

project(nativeui)

find_library(log-lib log)
find_library(android-lib android)
find_library(gles-lib GLESv2)
find_library(egl-lib EGL)

set(SOURCES
        native_app.cpp
        renderer.cpp
        shader_program.cpp
        ui_element.cpp
        bubble_button.cpp
        menu_button.cpp
        menu_layout.cpp
        animation.cpp
        math_utils.cpp
        time_utils.cpp
        android_native_app_glue.c
)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/android_native_app_glue
)

add_library(
        nativeui
        SHARED
        ${SOURCES}
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -ffast-math -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -O3 -ffast-math -funroll-loops -frtti -fexceptions")

# ---- android_native_app_glue korrekt einbinden ----
add_library(android_native_app_glue STATIC android_native_app_glue.c)
target_include_directories(android_native_app_glue PUBLIC
        ${CMAKE_SOURCE_DIR}/android_native_app_glue
)

target_link_libraries(
        nativeui
        ${log-lib}
        ${android-lib}
        ${gles-lib}
        ${egl-lib}
        android_native_app_glue
)