cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(HEADERS src/main.h src/VAO.h src/IBO.h src/VBO.h src/Shaders.h src/Renderer.h)

project(Solar VERSION 0.1.0 LANGUAGES C CXX)
add_executable(Solar src/main.cpp ${HEADERS})

find_package(OpenGl REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

target_link_libraries(Solar PRIVATE OpenGL::GL glfw glad::glad)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/build/libs")

add_library( imgui 
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp 
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp

)
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/build/include")

target_include_directories("imgui" PRIVATE ${INCLUDE_DIR})
target_include_directories(Solar PRIVATE ${INCLUDE_DIR})
target_link_libraries("imgui" PRIVATE glfw)
target_link_libraries(Solar PRIVATE "imgui")
