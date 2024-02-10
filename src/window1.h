#ifndef WINDOW1_H
#define WINDOW1_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "../imgui/implot.h"

#include <pqxx/pqxx>

#include <vector>
#include <unordered_map>
#include <iostream>
#include <numeric>
#include <cmath>

class Window1{
    public:
    static void window1_setup(bool* window_1);
    static void window1_body(bool* window_1, pqxx::result* result_for_table_tab1);
    static void window1_plots(pqxx::result* result_for_table_tab1);

    static float calculate_correlation(std::vector<int>* col_one, std::vector<int>* col_two);

};

#endif