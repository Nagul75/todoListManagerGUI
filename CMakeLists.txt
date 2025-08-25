cmake_minimum_required(VERSION 3.16)
project(todoListManagerGUI LANGUAGES CXX)

# --- Find GLFW and OpenGL ---
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# --- Define the ImGui sources ---
set(IMGUI_SOURCES
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_widgets.cpp
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
)

# --- Define main application ---
# It creates an executable named "TodoListGUI" from all listed source source files
add_executable(todoListManagerGUI
        main.cpp
        glad.c
        todoListManager.h
        todoListManager.cpp
        ${IMGUI_SOURCES}
)

# --- Configure the Target ---
# Add directories for the compiler to search for header files.
target_include_directories(todoListManagerGUI PRIVATE
        vendor/imgui
        backends
        .
)

# Link the executable against the libraries it needs.
target_link_libraries(todoListManagerGUI PRIVATE
        glfw
        ${OPENGL_LIBRARIES}
)

# Specify which files to install and where they should go.
# This installs final executable into a 'bin' folder in the package.
install(TARGETS todoListManagerGUI DESTINATION bin)

# This installs data file alongside the executable.
install(FILES tasks.txt DESTINATION bin)

# This module finds all the system libraries that app is dependent on (like libc++, etc.)
# and adds them to the installation.
include(InstallRequiredSystemLibraries)

# Set some basic package information.
set(CPACK_PACKAGE_NAME "todoListManagerGUI")
set(CPACK_PACKAGE_VERSION "1.0")
set(CPACK_PACKAGE_VENDOR "regal")

# Include the CPack module itself to enable packaging.
include(CPack)
