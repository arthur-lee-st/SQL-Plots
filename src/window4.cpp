#include "window4.h"

void Window4::window4_setup(bool* window_4){
/**
    auto xSize = ImGui::GetMainViewport()->Size.x - 200;
    auto yPos = ImGui::GetMainViewport()->Size.y / 2;
    ImGui::SetNextWindowSize(ImVec2(xSize, ImGui::GetIO().DisplaySize.y - yPos));
    ImGui::SetNextWindowPos(ImVec2(200, yPos));
*/
    auto main_window_dimentions = ImGui::GetMainViewport();
    auto yPos = ImGui::GetIO().DisplaySize.y;
    ImGui::SetNextWindowSize(ImVec2(main_window_dimentions->Size.x / 2, main_window_dimentions->Size.y - 19));
    ImGui::SetNextWindowPos(ImVec2(0, 19));
}

void Window4::window4_body(bool* window_4, char text1[256], char text2[256], bool* db_on_off, bool* show_hide_button, bool* fill_table, pqxx::result* result_raw, pqxx::result* result_for_table, pqxx::result* result_for_table_tab1, pqxx::result* result_for_table_tab2, bool* query1_success, bool* query2_success, int* radiobutton_choice_tab1_x, int* radiobutton_choice_tab1_y, int* radiobutton_choice_tab2_x, int* radiobutton_choice_tab2_y) {
    
    ImGui::Begin("Window 4", window_4, ImGuiWindowFlags_HorizontalScrollbar | //ImGuiWindowFlags_MenuBar
                                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    Window4::window_menu_bar(window_4);

    Window4::db_connection(db_on_off, show_hide_button, text1, text2, fill_table, result_raw, result_for_table, result_for_table_tab1, result_for_table_tab2, query1_success, query2_success, radiobutton_choice_tab1_x, radiobutton_choice_tab1_y, radiobutton_choice_tab2_x, radiobutton_choice_tab2_y);
    
    ImGui::End();
}

void Window4::window_menu_bar(bool* window_4) {
    if(ImGui::BeginMenuBar()){
        if(ImGui::BeginMenu("Menu")){
            if(ImGui::MenuItem("Close")){
                *window_4 = false;
            }
            ImGui::EndMenu();     
        }
        ImGui::EndMenuBar();
    }
    if(ImGui::IsKeyPressed(ImGuiKey_Q) & ImGui::IsWindowFocused()){
        *window_4 = false;
    }
}

void Window4::db_connection(bool* db_on_off, bool* show_hide_button, char text1[256], char text2[256], bool* fill_table, pqxx::result* result_raw, pqxx::result* result_for_table, pqxx::result* result_for_table_tab1, pqxx::result* result_for_table_tab2, bool* query1_success, bool* query2_success, int* radiobutton_choice_tab1_x, int* radiobutton_choice_tab1_y, int* radiobutton_choice_tab2_x, int* radiobutton_choice_tab2_y) {
    
    ImGui::Columns(2, "TwoCols", false);
    ImGui::Separator();
    bool button_connectdb = ImGui::Button("Connect Database");
    bool button_disconnectdb = ImGui::Button("Disconnect Database");
    //bool button_runquery = ImGui::Button("Run Query");
    ImGui::NextColumn();
    if(*db_on_off) {
        ImGui::Text("Database connected.");
    }
    else {
        ImGui::Text("Database disconnected.");
    }
    std::string tab1query = "tab1 query: " + result_for_table_tab1->query();
    if(result_for_table_tab1->query().length() != 0){
        ImGui::TextWrapped(tab1query.c_str());
    }
    std::string tab2query = "tab2 query: " + result_for_table_tab2->query();
    if(result_for_table_tab2->query().length() != 0){
        ImGui::TextWrapped(tab2query.c_str());
    }
    ImGui::NextColumn();
    ImGui::Columns(1);
    ImGui::Separator();

    if(!*db_on_off) {   //if db is off
        if(button_connectdb){
            std::cout << "button pressed to connect database." << std::endl;
            *db_on_off = true;
        }
        //keep the table visible even if database is disconnected.
        if(!result_for_table_tab1->empty()) {
            if(ImGui::BeginTabBar("Tab Bar 1 2 DB Off")){
                if(ImGui::BeginTabItem("Tab 1")){
                    Window4::populate_table(fill_table, text1, result_for_table_tab1);
                    ImGui::EndTabItem();
                }
                if(ImGui::BeginTabItem("Tab 2")){
                    Window4::populate_table(fill_table, text2, result_for_table_tab2);
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        }
    }
    else {  //if db is on
        pqxx::connection conn{"dbname=SQLPracticeDB1 user=dbuser password=dbpassword hostaddr=127.0.0.1 port=5432"};
        Postgresql1 db{&conn};        

        bool button_show_hide = ImGui::Button("Show/Hide");

        //following if/else if and then another if: to control show/hide the sql query input textbox and "run query button."
        if(button_show_hide && *show_hide_button == false){ //if button is pressed and input textbox is currently hidden, then unhide it
            *show_hide_button = true;
        }
        else if(button_show_hide && *show_hide_button == true) { //if button is pressed and input textbox is currently visible, then hide it
            *show_hide_button = false;
        }

        //tab1 and tab2
        if(ImGui::BeginTabBar("Tab Bar 1 2 DB On")) {
            if(ImGui::BeginTabItem("Tab 1")){
                if(*show_hide_button) {    
                    ImGui::Columns(2, "TC1", false);
                    bool input_text_query1 = ImGui::InputTextMultiline("##sql_input_box1", text1, 256, ImVec2(230, 150));
                    if(input_text_query1){
                        std::cout << text1 << std::endl;
                    }
                    ImGui::NextColumn();
                    if(*query1_success == false && result_for_table_tab1->empty()){ //if query failed and result is empty
                        ImGui::Text("Query 1 failed. Try again.");
                    }
                    else if(*query1_success == true && result_for_table_tab1->query() != ""){ //if query success and input text not empty
                        ImGui::Text("Query 1 success.");
                    }
                    else if (*query1_success == false && result_for_table_tab1->query() != ""){ //if query failed and input text not empty
                        ImGui::Text("Query 1 failed. Try again.");
                    }
                    else {
                    }
                    ImGui::NextColumn();
                    ImGui::Columns(1);
                    if(ImGui::Button("Run Query")) {
                        *radiobutton_choice_tab1_x = 0;
                        *radiobutton_choice_tab1_y = 0;
                        *result_for_table_tab1 = db.postgresql_body(text1, result_raw, result_for_table_tab1, query1_success);
                        *fill_table = true;
                    }
                }
                Window4::populate_table(fill_table, text1, result_for_table_tab1);
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Tab 2")){
                if(*show_hide_button) {    
                    ImGui::Columns(2, "TC2", false);
                    bool input_text_query2 = ImGui::InputTextMultiline("##sql_input_box2", text2, 256, ImVec2(230, 150));
                    if(input_text_query2){
                        std::cout << text2 << std::endl;
                    }
                    ImGui::NextColumn();
                    if(*query2_success == false && result_for_table_tab2->empty()){ //if query failed and result is empty
                        ImGui::Text("Query 2 failed. Try again.");
                    }
                    else if(*query2_success == true && result_for_table_tab2->query() != ""){ //if query success and input text not empty
                        ImGui::Text("Query 2 success.");
                    }
                    else if (*query2_success == false && result_for_table_tab2->query() != ""){ //if query failed and input text not empty
                        ImGui::Text("Query 2 failed. Try again.");
                    }
                    else {
                    }
                    ImGui::NextColumn();
                    ImGui::Columns(1);
                    if(ImGui::Button("Run Query")) {
                        *radiobutton_choice_tab2_x = 0;
                        *radiobutton_choice_tab2_y = 0;
                        *result_for_table_tab2 = db.postgresql_body(text2, result_raw, result_for_table_tab2, query2_success);
                        *fill_table = true;
                    }
                }
                Window4::populate_table(fill_table, text2, result_for_table_tab2);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        if(button_disconnectdb){
            std::cout << "button pressed to disconnect database." << std::endl;
            db.postgresql_turn_off(db_on_off);
        }
    }
}

//Display Sql results in ImGui table
void Window4::populate_table(bool* fill_table, char text_to_populate[256], pqxx::result* result_to_populate) {

    //if result 
    if(result_to_populate->columns() > 0){
        ImGui::BeginTable("table4", result_to_populate->columns(), ImGuiTableFlags_Resizable | 
                                        ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY);
        
        if (*fill_table) {
            //fill out table column names
            for(int col_index = 0; col_index < result_to_populate->columns(); ++col_index){
                ImGui::TableSetupColumn(result_to_populate->column_name(col_index), ImGuiTableColumnFlags_WidthFixed);
            }
            ImGui::TableHeadersRow();

            //populate table cells with data
            for (auto const &row : *result_to_populate){
                ImGui::TableNextRow();
                int columnindex = 0;
                for(auto const &field : row){
                    ImGui::TableSetColumnIndex(columnindex);
                    ImGui::Text(field.c_str());
                    columnindex++;
                }
            }
        }       

        ImGui::EndTable();    
 
    }
    else {  
        //ImGui::BeginTable("table4", 1, ImGuiTableFlags_Resizable | 
        //                                ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY);
    }
}

