#ifndef WINDOW5_H
#define WINDOW5_H

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

class Window5{
    public:
    static void window5_setup(bool* window_5);
    static void window5_body(bool* window_5, pqxx::result* result_for_table_tab2);
    static void window5_plots(pqxx::result* result_for_table_tab2);

    static float calculate_correlation(std::vector<int>* col_one, std::vector<int>* col_two);

};

#endif