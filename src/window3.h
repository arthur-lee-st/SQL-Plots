#ifndef WINDOW3_H
#define WINDOW3_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "../imgui/implot.h"

#include <pqxx/pqxx>

#include <unordered_map>
#include <string>
#include <iostream>


class Window3{
    public:

    static void window3_setup(bool* window_3);
    static void window3_body(bool* window_3, char inputtext_window3[64], pqxx::result* result_for_table_tab1, int* radiobutton_choice_tab1_x, int* radiobutton_choice_tab1_y, bool* plotbar_checked_tab1, bool* plotscatter_checked_tab1);
    static void window3_plots(pqxx::result* result_for_table_tab1, int* radiobutton_choice_tab1_x, int* radiobutton_choice_tab1_y, bool* plotbar_checked_tab1, bool* plotscatter_checked_tab1);

    //helper functions
    static void window3_getcolumnheadernames(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names);
    static void window3_createradiobuttons_x(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab1_x);
    static void window3_createradiobuttons_y(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab1_y);
    static void window3_getcolumnnameofidofradiobutton_x(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab1_x, std::vector<int>* data_vector_tab1_x);
    static void window3_getcolumnnameofidofradiobutton_y(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab1_y, std::vector<int>* data_vector_tab1_y);
    static int window3_findminvalue(std::vector<int>* barplot_data_vector);
    static int window3_findmaxvalue(std::vector<int>* barplot_data_vector);

};

#endif