#ifndef WINDOW4_H
#define WINDOW4_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <string>

#include "postgresql1.h"

class Window4{
    public:

    static void window4_setup(bool* window_4);
    static void window4_body(bool* window_4, char text1[256], char text2[256], bool* db_on_off, bool* show_hide_button, bool* fill_table, pqxx::result* result_raw, pqxx::result* result_for_table, pqxx::result* result_for_table_tab1, pqxx::result* result_for_table_tab2, bool* query1_success, bool* query2_success, int* radiobutton_choice_tab1_x, int* radiobutton_choice_tab1_y, int* radiobutton_choice_tab2_x, int* radiobutton_choice_tab2_y);

    private:
    static void window_menu_bar(bool* window_4);
    static void db_connection(bool* db_on_off, bool* show_hide_button, char text1[256], char text2[256], bool* fill_table, pqxx::result* result_raw, pqxx::result* result_for_table, pqxx::result* result_for_table_tab1, pqxx::result* result_for_table_tab2, bool* query1_success, bool* query2_success, int* radiobutton_choice_tab1_x, int* radiobutton_choice_tab1_y, int* radiobutton_choice_tab2_x, int* radiobutton_choice_tab2_y);
    static void populate_table(bool* fill_table, char text_to_populate[256], pqxx::result* result_to_populate);
};

#endif