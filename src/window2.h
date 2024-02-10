#ifndef WINDOW2_H
#define WINDOW2_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "../imgui/implot.h"

#include <pqxx/pqxx>

#include <unordered_map>
#include <string>
#include <iostream>

class Window2{
    public:

    static void window2_setup(bool* window_2);
    static void window2_body(bool* window_2, pqxx::result* result_for_table_tab2, int* radiobutton_choice_tab2_x, int* radiobutton_choice_tab2_y, bool* plotbar_checked_tab2, bool* plotscatter_checked_tab2);
    static void window2_plots(pqxx::result* result_for_table_tab2, int* radiobutton_choice_tab2_x, int* radiobutton_choice_tab2_y, bool* plotbar_checked_tab2, bool* plotscatter_checked_tab2);

    //helper functions
    static void window2_getcolumnheadernames(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names);
    static void window2_createradiobuttons_x(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab2_x);
    static void window2_createradiobuttons_y(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab2_y);
    static void window2_getcolumnnameofidofradiobutton_x(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab2_x, std::vector<int>* data_vector_tab2_x);
    static void window2_getcolumnnameofidofradiobutton_y(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab2_y, std::vector<int>* data_vector_tab2_y);
    static int window2_findminvalue(std::vector<int>* barplot_data_vector);
    static int window2_findmaxvalue(std::vector<int>* barplot_data_vector);
};

#endif