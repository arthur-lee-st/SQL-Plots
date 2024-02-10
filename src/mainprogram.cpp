#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "../imgui/implot.h"

#include <pqxx/pqxx>
#include "../src/postgresql1.h"

//#include "../src/window1.h"
#include "../src/window2.h"
#include "../src/window3.h"
#include "../src/window4.h" //window for postgresql
//#include "../src/window5.h"
#include "../src/postgresql1.h"

#include <iostream>
#include <string>
#include <unordered_map>

int mainWindowWidth = 1000;
int mainWindowHeight = 600;

int main(int argc, char** argv) {

    //std::cout << "imgui running" << std::endl;

    if(!glfwInit()) {
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(mainWindowWidth, mainWindowHeight, "SQL Plots", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    //initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    /**
     * windows - true means to display window. if code sets it to false, then window will not display
     */ 
    bool main_toolbar = true;
    bool windows_controller = true;

    //window 3 - top left window
    bool window_3 = true;
    static char inputtext_window3[64] = "text"; //used for textbox
    bool plotbar_checked_tab1 = false;
    bool plotscatter_checked_tab1 = false;
    static int radiobutton_choice_tab1_x = 0;
    static int radiobutton_choice_tab1_y = 0;

    //window 1 - bottom left window
    bool window_1 = true;

    //window 2 - top right window
    bool window_2 = true;
    bool plotbar_checked_tab2 = false;
    bool plotscatter_checked_tab2 = false;
    static int radiobutton_choice_tab2_x = 0;
    static int radiobutton_choice_tab2_y = 0;

    //window 4 - leftmost window
    bool window_4 = true;
    Postgresql1* db;
    bool db_on_off = false; //db is false because once when this c++ program first starts, the db is off
    bool show_hide_button = false;
    bool fill_table = false;
    char text1[256] = "type query here";
    char text2[256] = "type query here";
    pqxx::result result_raw;
    pqxx::result result_for_table;
    pqxx::result result_for_table_tab1;
    pqxx::result result_for_table_tab2;
    bool query1_success = true;
    bool query2_success = true;

    //window 5 - bottom right window
    bool window_5 = true;

    //variables
    bool finished = false;  //used for checkbox

    //start postgresql
    //Postgresql1::postgresql_setup(&db_on_off);

    //main loop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //menu bar functions
        if(main_toolbar){
            ImGui::BeginMainMenuBar();
            if(ImGui::BeginMenu("File")){
                if(ImGui::MenuItem("Close")){
                    break; //exits main while loop thus closing the program
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Edit")){
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("View")){
                //if(ImGui::MenuItem("Show Window 1")){
                //    window_1 = true;
                //}
                if(ImGui::MenuItem("Show Window Left")){
                    window_4 = true;
                }
                if(ImGui::MenuItem("Show Window Top Right")){
                    window_3 = true;
                }
                if(ImGui::MenuItem("Show Window Bottom Right")){
                    window_2 = true;
                }
                //if(ImGui::MenuItem("Show Window 5")){
                //    window_5 = true;
                //}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if(!windows_controller){
            ImGui::SetNextWindowSize(ImVec2(200, ImGui::GetMainViewport()->Size.y));
            ImGui::SetNextWindowPos(ImVec2(0, 19));
            ImGui::Begin("Windows Controller", &windows_controller, ImGuiWindowFlags_NoResize);
            ImGui::End();
        }

        if(window_1) {
        //    Window1::window1_setup(&window_1);
        //    Window1::window1_body(&window_1, &result_for_table_tab1);            
        }

        if(window_2) {
            Window2::window2_setup(&window_2);
            Window2::window2_body(&window_2, &result_for_table_tab2, &radiobutton_choice_tab2_x, &radiobutton_choice_tab2_y, &plotbar_checked_tab2, &plotscatter_checked_tab2);
        }

        if(window_3) {
            Window3::window3_setup(&window_3);
            Window3::window3_body(&window_3, inputtext_window3, &result_for_table_tab1, &radiobutton_choice_tab1_x, &radiobutton_choice_tab1_y, &plotbar_checked_tab1, &plotscatter_checked_tab1);
        }

        if(window_4) {
            Window4::window4_setup(&window_4);
            Window4::window4_body(&window_4, text1, text2, &db_on_off, &show_hide_button, &fill_table, &result_raw, &result_for_table, &result_for_table_tab1, &result_for_table_tab2, &query1_success, &query2_success, &radiobutton_choice_tab1_x, &radiobutton_choice_tab1_y, &radiobutton_choice_tab2_x, &radiobutton_choice_tab2_y);
        }

        if(window_5){
        //    Window5::window5_setup(&window_5);
        //    Window5::window5_body(&window_5, &result_for_table_tab2);
        }

        ImGui::Render();
        int display_w;
        int display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0,0,display_w,display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    //cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}