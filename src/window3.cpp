#include "window3.h"

void Window3::window3_setup(bool* window_3){
/**
    auto yPos = ImGui::GetIO().DisplaySize.y / 2;
    ImGui::SetNextWindowSize(ImVec2((ImGui::GetMainViewport()->Size.x-200) / 3, yPos - 19));
    ImGui::SetNextWindowPos(ImVec2(((ImGui::GetMainViewport()->Size.x-200) / 3 * 2) + 200, 19));
*/
    auto main_window_dimentions = ImGui::GetMainViewport();
    auto yPos = ImGui::GetIO().DisplaySize.y;
    auto window4width = main_window_dimentions->Size.x / 2;
    ImGui::SetNextWindowSize(ImVec2(main_window_dimentions->Size.x-window4width, (yPos-19)/2));
    ImGui::SetNextWindowPos(ImVec2(main_window_dimentions->Size.x / 2, 19));
}
    

void Window3::window3_body(bool* window_3, char inputtext_window3[64], pqxx::result* result_for_table_tab1, int* radiobutton_choice_tab1_x, int* radiobutton_choice_tab1_y, bool* plotbar_checked_tab1, bool* plotscatter_checked_tab1) {

    ImPlot::CreateContext();
    ImGui::Begin("Window 3", window_3, ImGuiWindowFlags_HorizontalScrollbar | //ImGuiWindowFlags_MenuBar
                                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    if(ImGui::BeginMenuBar()){
        if(ImGui::BeginMenu("Menu")){
            if(ImGui::MenuItem("Close")){
                *window_3 = false;
            }
            ImGui::EndMenu();     
        }
        ImGui::EndMenuBar();
    }
    if(ImGui::IsKeyPressed(ImGuiKey_Q) & ImGui::IsWindowFocused()){
        *window_3 = false;
    }

    Window3::window3_plots(result_for_table_tab1, radiobutton_choice_tab1_x, radiobutton_choice_tab1_y, plotbar_checked_tab1, plotscatter_checked_tab1);    

    ImGui::End();
    ImPlot::DestroyContext();

}

void Window3::window3_plots(pqxx::result* result_for_table_tab1, int* radiobutton_choice_tab1_x, int* radiobutton_choice_tab1_y, bool* plotbar_checked_tab1, bool* plotscatter_checked_tab1){
    
    std::unordered_map<int, std::string> column_header_names;

    //get the column header names
    Window3::window3_getcolumnheadernames(result_for_table_tab1, &column_header_names);
    
    //create the radio buttons for x axis and y axis
    Window3::window3_createradiobuttons_x(result_for_table_tab1, &column_header_names, radiobutton_choice_tab1_x);
    Window3::window3_createradiobuttons_y(result_for_table_tab1, &column_header_names, radiobutton_choice_tab1_y);

    //get the columnname of id of selected radiobutton_choice_tab1_x and radiobutton_choice_tab1_y. then grab the column data and store it in vector
    std::vector<int> data_vector_tab1_x;
    Window3::window3_getcolumnnameofidofradiobutton_x(result_for_table_tab1, &column_header_names, radiobutton_choice_tab1_x, &data_vector_tab1_x);

    std::vector<int> data_vector_tab1_y;
    Window3::window3_getcolumnnameofidofradiobutton_y(result_for_table_tab1, &column_header_names, radiobutton_choice_tab1_y, &data_vector_tab1_y);



    //copy over the data vector to an array in order to feed it into ImPlot::PlotBars 
    //then plot it
    const int* data_array_tab1_x;
    int minvalue_x;
    int maxvalue_x;
    if(!data_vector_tab1_x.empty()) {
        data_array_tab1_x = data_vector_tab1_x.data();
        //get min and max values in data vector
        minvalue_x = Window3::window3_findminvalue(&data_vector_tab1_x);
        maxvalue_x = Window3::window3_findmaxvalue(&data_vector_tab1_x);
    }

    const int* data_array_tab1_y;
    int minvalue_y;
    int maxvalue_y;
    if(!data_vector_tab1_y.empty()) {
        data_array_tab1_y = data_vector_tab1_y.data();
        //get min and max values in data vector
        minvalue_y = Window3::window3_findminvalue(&data_vector_tab1_y);
        maxvalue_y = Window3::window3_findmaxvalue(&data_vector_tab1_y);
    }  

    bool plotbar_checkbox = ImGui::Checkbox("Bars", plotbar_checked_tab1); 
    ImGui::SameLine();
    bool plotscatter_checkbox = ImGui::Checkbox("Scatter", plotscatter_checked_tab1);

    /**
    * controls the checkbox's checked image
    */
    if(plotbar_checkbox && *plotbar_checked_tab1) { //plotbar's checkbox is on, plotscatter's checkbox is off
        *plotbar_checked_tab1 = true;
        *plotscatter_checked_tab1 = false;
    }
    else if(plotbar_checkbox && !*plotbar_checked_tab1){    //plotbar's checkbox is off, plotscatter's checkbox is off
        *plotbar_checked_tab1 = false;
        *plotscatter_checked_tab1 = false;
    }
    if(plotscatter_checkbox && *plotscatter_checked_tab1) { //plotbar's checkbox is off, plotscatter's checkbox is on
        *plotscatter_checked_tab1 = true;
        *plotbar_checked_tab1 = false;
    }
    else if(plotscatter_checkbox && !*plotscatter_checked_tab1){  //plotbar's checkbox is off, plotscatter's checkbox is off
        *plotscatter_checked_tab1 = false;
        *plotbar_checked_tab1 = false;
    }

    if(*plotbar_checked_tab1 && !*plotscatter_checked_tab1) {  //plotbar only
        if(!data_vector_tab1_y.empty()){
            std::string plotname = column_header_names.at(*radiobutton_choice_tab1_y);
            if(ImPlot::BeginPlot(plotname.c_str(), ImVec2(400, 230))){
                ImPlot::SetupAxesLimits(1, result_for_table_tab1->size(), minvalue_y, maxvalue_y);
                ImPlot::SetupAxis(ImAxis_X1, "row");
                ImPlot::SetupAxis(ImAxis_Y1, column_header_names.at(*radiobutton_choice_tab1_y).c_str());
                ImPlot::PlotBars("##MyBarPlot", data_array_tab1_y, result_for_table_tab1->size(), 0.7, 1);
                ImPlot::EndPlot();
            }  
        }
    }
    else if(!*plotbar_checked_tab1 && *plotscatter_checked_tab1) { //plotscatter only
        if(!data_vector_tab1_x.empty() && !data_vector_tab1_y.empty()) {
            std::string plotname = column_header_names.at(*radiobutton_choice_tab1_y) + " vs " + column_header_names.at(*radiobutton_choice_tab1_x);
            if(ImPlot::BeginPlot(plotname.c_str(), ImVec2(400, 230))){
                ImPlot::SetupAxesLimits(minvalue_x, maxvalue_x, minvalue_y, maxvalue_y);
                ImPlot::SetupAxis(ImAxis_X1, column_header_names.at(*radiobutton_choice_tab1_x).c_str());
                ImPlot::SetupAxis(ImAxis_Y1, column_header_names.at(*radiobutton_choice_tab1_y).c_str());
                ImPlot::PlotScatter("##Scatterplot", data_array_tab1_x, data_array_tab1_y, result_for_table_tab1->size(), 0.7);
                ImPlot::EndPlot();
            }
        }    
    }    
    else {
        ImGui::Columns(2, "Window3_TextCol", false);
        ImGui::SetColumnWidth(0, ImGui::GetIO().DisplaySize.x - 10);
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::TextWrapped("Type in SQL query in left box then select the plot option to visualize results.");
        ImGui::Separator();
        ImGui::Columns(1);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////HELPER FUNCTIONS/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void Window3::window3_getcolumnheadernames(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names){
    if(column_header_names->size() < result_for_table_tab1->columns()) {
        for(int col_index = 0; col_index < result_for_table_tab1->columns(); ++col_index){
            column_header_names->insert(std::make_pair(col_index, result_for_table_tab1->column_name(col_index)));
        }
    }
}

void Window3::window3_createradiobuttons_x(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab1_x) {
    if(!column_header_names->empty() && column_header_names->size() == result_for_table_tab1->columns()) {
        for(int i = 0; i < column_header_names->size(); ++i) {
            ImGui::PushID(i);
            std::string headername = column_header_names->at(i) + "_x";
            ImGui::RadioButton(headername.c_str(), radiobutton_choice_tab1_x, i); 
            if (i + 1 < column_header_names->size()) {
                ImGui::SameLine();
            }
            ImGui::PopID();
        }
    }
}

void Window3::window3_createradiobuttons_y(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab1_y) {
    if(!column_header_names->empty() && column_header_names->size() == result_for_table_tab1->columns()) {
        for(int j = 0; j < column_header_names->size(); ++j) {
            ImGui::PushID(j);
            std::string headername = column_header_names->at(j) + "_y";
            ImGui::RadioButton(headername.c_str(), radiobutton_choice_tab1_y, j);
            if (j + 1 < column_header_names->size()) {
                ImGui::SameLine();
            }          
            ImGui::PopID();      
        }
    }
}

void Window3::window3_getcolumnnameofidofradiobutton_x(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab1_x, std::vector<int>* data_vector_tab1_x) {
    for (auto const &row : *result_for_table_tab1){
        for(auto const &field : row){
            if(field.name() == column_header_names->at(*radiobutton_choice_tab1_x)) {
                data_vector_tab1_x->push_back(std::atoi(field.c_str()));
            }
        }
    }
}

void Window3::window3_getcolumnnameofidofradiobutton_y(pqxx::result* result_for_table_tab1, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab1_y, std::vector<int>* data_vector_tab1_y) {
    for (auto const &row : *result_for_table_tab1){
        for(auto const &field : row){
            if(field.name() == column_header_names->at(*radiobutton_choice_tab1_y)) {
                data_vector_tab1_y->push_back(std::atoi(field.c_str()));
            }
        }
    }
}

int Window3::window3_findminvalue(std::vector<int>* barplot_data_vector) {
    /**
    * get the min value and max value from data vector
    */
    int minvalue;
    minvalue = barplot_data_vector->at(0);
    for (int i = 1; i < barplot_data_vector->size(); ++i) {
        if(barplot_data_vector->at(i) < minvalue) {
            minvalue = barplot_data_vector->at(i);
        }
    }
    return minvalue;
}

int Window3::window3_findmaxvalue(std::vector<int>* barplot_data_vector) {
    /**
    * get the min value and max value from data vector
    */
    int maxvalue;
    maxvalue = barplot_data_vector->at(0);
    for (int i = 1; i < barplot_data_vector->size(); ++i) {
        if(barplot_data_vector->at(i) > maxvalue) {
            maxvalue = barplot_data_vector->at(i);
        }
    }
    return maxvalue;
}