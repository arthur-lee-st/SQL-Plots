#include "window2.h"

void Window2::window2_setup(bool* window_2) {

    auto main_window_dimentions = ImGui::GetMainViewport();
    auto yPos = ImGui::GetIO().DisplaySize.y;
    auto window4width = main_window_dimentions->Size.x / 2;
    ImGui::SetNextWindowSize(ImVec2(main_window_dimentions->Size.x - window4width, (yPos-19)/2));
    ImGui::SetNextWindowPos(ImVec2(main_window_dimentions->Size.x / 2, (yPos-19)/2 + 19));
}


void Window2::window2_body(bool* window_2, pqxx::result* result_for_table_tab2, int* radiobutton_choice_tab2_x, int* radiobutton_choice_tab2_y, bool* plotbar_checked_tab2, bool* plotscatter_checked_tab2) {

    ImPlot::CreateContext();
    ImGui::Begin("Window 2", window_2, ImGuiWindowFlags_HorizontalScrollbar | //ImGuiWindowFlags_MenuBar
                                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    if(ImGui::BeginMenuBar()){
        if(ImGui::BeginMenu("Menu")){
            if(ImGui::MenuItem("Close")){
                *window_2 = false;
            }
            ImGui::EndMenu();     
        }
        ImGui::EndMenuBar();
    }
    if(ImGui::IsKeyPressed(ImGuiKey_Q) & ImGui::IsWindowFocused()){
        *window_2 = false;
    }

    Window2::window2_plots(result_for_table_tab2, radiobutton_choice_tab2_x, radiobutton_choice_tab2_y, plotbar_checked_tab2, plotscatter_checked_tab2);    

    ImGui::End();
    ImPlot::DestroyContext();

}

void Window2::window2_plots(pqxx::result* result_for_table_tab2, int* radiobutton_choice_tab2_x, int* radiobutton_choice_tab2_y, bool* plotbar_checked_tab2, bool* plotscatter_checked_tab2){
    
    std::unordered_map<int, std::string> column_header_names;

    //get the column header names
    Window2::window2_getcolumnheadernames(result_for_table_tab2, &column_header_names);
    
    //create the radio buttons for x axis and y axis
    Window2::window2_createradiobuttons_x(result_for_table_tab2, &column_header_names, radiobutton_choice_tab2_x);
    Window2::window2_createradiobuttons_y(result_for_table_tab2, &column_header_names, radiobutton_choice_tab2_y);

    //get the columnname of id of selected radiobutton_choice_tab2_x and radiobutton_choice_tab2_y. then grab the column data and store it in vector
    std::vector<int> data_vector_tab2_x;
    Window2::window2_getcolumnnameofidofradiobutton_x(result_for_table_tab2, &column_header_names, radiobutton_choice_tab2_x, &data_vector_tab2_x);

    std::vector<int> data_vector_tab2_y;
    Window2::window2_getcolumnnameofidofradiobutton_y(result_for_table_tab2, &column_header_names, radiobutton_choice_tab2_y, &data_vector_tab2_y);



    //copy over the data vector to an array in order to feed it into ImPlot::PlotBars 
    //then plot it
    const int* data_array_tab2_x;
    int minvalue_x;
    int maxvalue_x;
    if(!data_vector_tab2_x.empty()) {
        data_array_tab2_x = data_vector_tab2_x.data();
        //get min and max values in data vector
        minvalue_x = Window2::window2_findminvalue(&data_vector_tab2_x);
        maxvalue_x = Window2::window2_findmaxvalue(&data_vector_tab2_x);
    }

    const int* data_array_tab2_y;
    int minvalue_y;
    int maxvalue_y;
    if(!data_vector_tab2_y.empty()) {
        data_array_tab2_y = data_vector_tab2_y.data();
        //get min and max values in data vector
        minvalue_y = Window2::window2_findminvalue(&data_vector_tab2_y);
        maxvalue_y = Window2::window2_findmaxvalue(&data_vector_tab2_y);
    }  

    bool plotbar_checkbox = ImGui::Checkbox("Bars", plotbar_checked_tab2); 
    ImGui::SameLine();
    bool plotscatter_checkbox = ImGui::Checkbox("Scatter", plotscatter_checked_tab2);

    /**
    * controls the checkbox's checked image
    */
    if(plotbar_checkbox && *plotbar_checked_tab2) { //plotbar's checkbox is on, plotscatter's checkbox is off
        *plotbar_checked_tab2 = true;
        *plotscatter_checked_tab2 = false;
    }
    else if(plotbar_checkbox && !*plotbar_checked_tab2){    //plotbar's checkbox is off, plotscatter's checkbox is off
        *plotbar_checked_tab2 = false;
        *plotscatter_checked_tab2 = false;
    }
    if(plotscatter_checkbox && *plotscatter_checked_tab2) { //plotbar's checkbox is off, plotscatter's checkbox is on
        *plotscatter_checked_tab2 = true;
        *plotbar_checked_tab2 = false;
    }
    else if(plotscatter_checkbox && !*plotscatter_checked_tab2){  //plotbar's checkbox is off, plotscatter's checkbox is off
        *plotscatter_checked_tab2 = false;
        *plotbar_checked_tab2 = false;
    }

    if(*plotbar_checked_tab2 && !*plotscatter_checked_tab2) {  //plotbar only
        if(!data_vector_tab2_y.empty()){
            std::string plotname = column_header_names.at(*radiobutton_choice_tab2_y);
            if(ImPlot::BeginPlot(plotname.c_str(), ImVec2(400, 230))){
                ImPlot::SetupAxesLimits(1, result_for_table_tab2->size(), minvalue_y, maxvalue_y);
                ImPlot::SetupAxis(ImAxis_X1, "row");
                ImPlot::SetupAxis(ImAxis_Y1, column_header_names.at(*radiobutton_choice_tab2_y).c_str());
                ImPlot::PlotBars("##MyBarPlot", data_array_tab2_y, result_for_table_tab2->size(), 0.7, 1);
                ImPlot::EndPlot();
            }  
        }
    }
    else if(!*plotbar_checked_tab2 && *plotscatter_checked_tab2) { //plotscatter only
        if(!data_vector_tab2_x.empty() && !data_vector_tab2_y.empty()) {
            std::string plotname = column_header_names.at(*radiobutton_choice_tab2_y) + " vs " + column_header_names.at(*radiobutton_choice_tab2_x);
            if(ImPlot::BeginPlot(plotname.c_str(), ImVec2(400, 230))){
                ImPlot::SetupAxesLimits(minvalue_x, maxvalue_x, minvalue_y, maxvalue_y);
                ImPlot::SetupAxis(ImAxis_X1, column_header_names.at(*radiobutton_choice_tab2_x).c_str());
                ImPlot::SetupAxis(ImAxis_Y1, column_header_names.at(*radiobutton_choice_tab2_y).c_str());
                ImPlot::PlotScatter("##Scatterplot", data_array_tab2_x, data_array_tab2_y, result_for_table_tab2->size(), 0.7);
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

void Window2::window2_getcolumnheadernames(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names){
    if(column_header_names->size() < result_for_table_tab2->columns()) {
        for(int col_index = 0; col_index < result_for_table_tab2->columns(); ++col_index){
            column_header_names->insert(std::make_pair(col_index, result_for_table_tab2->column_name(col_index)));
        }
    }
}

void Window2::window2_createradiobuttons_x(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab2_x) {
    if(!column_header_names->empty() && column_header_names->size() == result_for_table_tab2->columns()) {
        for(int i = 0; i < column_header_names->size(); ++i) {
            ImGui::PushID(i);
            std::string headername = column_header_names->at(i) + "_x";
            ImGui::RadioButton(headername.c_str(), radiobutton_choice_tab2_x, i); 
            if (i + 1 < column_header_names->size()) {
                ImGui::SameLine();
            }
            ImGui::PopID();
        }
    }
}

void Window2::window2_createradiobuttons_y(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab2_y) {
    if(!column_header_names->empty() && column_header_names->size() == result_for_table_tab2->columns()) {
        for(int j = 0; j < column_header_names->size(); ++j) {
            ImGui::PushID(j);
            std::string headername = column_header_names->at(j) + "_y";
            ImGui::RadioButton(headername.c_str(), radiobutton_choice_tab2_y, j);
            if (j + 1 < column_header_names->size()) {
                ImGui::SameLine();
            }          
            ImGui::PopID();      
        }
    }
}

void Window2::window2_getcolumnnameofidofradiobutton_x(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab2_x, std::vector<int>* data_vector_tab2_x) {
    for (auto const &row : *result_for_table_tab2){
        for(auto const &field : row){
            if(field.name() == column_header_names->at(*radiobutton_choice_tab2_x)) {
                data_vector_tab2_x->push_back(std::atoi(field.c_str()));
            }
        }
    }
}

void Window2::window2_getcolumnnameofidofradiobutton_y(pqxx::result* result_for_table_tab2, std::unordered_map<int, std::string>* column_header_names, int* radiobutton_choice_tab2_y, std::vector<int>* data_vector_tab2_y) {
    for (auto const &row : *result_for_table_tab2){
        for(auto const &field : row){
            if(field.name() == column_header_names->at(*radiobutton_choice_tab2_y)) {
                data_vector_tab2_y->push_back(std::atoi(field.c_str()));
            }
        }
    }
}

int Window2::window2_findminvalue(std::vector<int>* barplot_data_vector) {
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

int Window2::window2_findmaxvalue(std::vector<int>* barplot_data_vector) {
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