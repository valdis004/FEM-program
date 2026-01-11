cmake_minimum_required(VERSION 3.23)

set(CORE_PROJECT_NAME opengl)

add_library(${CORE_PROJECT_NAME} STATIC src/glfw.cpp include/glfw.h)

target_include_directories(${CORE_PROJECT_NAME} PUBLIC include)

add_subdirectory(${CMAKE_HOME_DIRECTORY}/external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(${CMAKE_HOME_DIRECTORY}/external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glad)