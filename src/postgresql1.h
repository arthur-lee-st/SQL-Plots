#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <pqxx/pqxx>

#include <iostream>
#include <string>

class Postgresql1{
    public:

    pqxx::connection* conn_ptr;

    //constructor
    Postgresql1(pqxx::connection* conn){
        conn_ptr = conn;
    };

    void postgresql_turn_on(bool* db_on_off);
    pqxx::result postgresql_body(char text[256], pqxx::result* result_raw, pqxx::result* result_for_table, bool* query_success);
    void postgresql_turn_off(bool* db_on_off);

};

#endif