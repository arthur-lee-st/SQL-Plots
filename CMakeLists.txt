cmake_minimum_required(VERSION 3.18)
set(PROJECTNAME SQL_Plots)
project(${PROJECTNAME} VERSION 0.0.1)

find_package(OpenGL REQUIRED)

add_library(LibsModule 
    imgui/glad.c
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/main_menu_tool_bar.h
    imgui/main_menu_tool_bar.cpp
    imgui/imgui.h
    imgui/imgui.cpp
    imgui/imgui_impl_glfw.h
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.h
    imgui/imgui_impl_opengl3.cpp
    imgui/imgui_impl_opengl3_loader.h
    imgui/imgui_internal.h
    imgui/imconfig.h
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/implot.h
    imgui/implot.cpp
    imgui/implot_internal.h
    imgui/implot_items.cpp
    src/postgresql1.h
    src/postgresql1.cpp
    src/window1.h
    src/window1.cpp
    src/window2.h
    src/window2.cpp
    src/window3.h
    src/window3.cpp
    src/window4.h
    src/window4.cpp
    src/window5.h
    src/window5.cpp
    
)

include_directories (../imgui/)

add_executable(${PROJECTNAME} ../src/mainprogram.cpp )

target_link_libraries(${PROJECTNAME} LibsModule )
target_link_libraries(${PROJECTNAME} 
    -lglfw 
    -lGL 
    -lGLU 
    -ldl 
    -lpthread 
    -lX11 #provides basic framework for GUI environment, draw and move windows on device display, interact with mouse and keyboard
    -lXrandr #resize and rotate, set size, orientation, reflection of the outputs for a screen
    -lpqxx
    -lpq
) 