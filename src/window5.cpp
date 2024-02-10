#include "window5.h"

void Window5::window5_setup(bool* window_5){
    auto main_window_dimentions = ImGui::GetMainViewport();
    auto yPos = ImGui::GetIO().DisplaySize.y;
    ImGui::SetNextWindowSize(ImVec2((main_window_dimentions->Size.x) / 3, (yPos-19)/2));
    ImGui::SetNextWindowPos(ImVec2(main_window_dimentions->Size.x / 3 * 2, (yPos-19)/2 + 19));
}

void Window5::window5_body(bool* window_5, pqxx::result* result_for_table_tab2) {
    ImPlot::CreateContext();
    ImGui::Begin("Window 5", window_5, ImGuiWindowFlags_HorizontalScrollbar | //ImGuiWindowFlags_MenuBar
                                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    //menu bar functions
    if(ImGui::BeginMenuBar()){
        if(ImGui::BeginMenu("Menu")){
            if(ImGui::MenuItem("Close")){
                *window_5 = false;
            }
            ImGui::EndMenu();     
        }
        ImGui::EndMenuBar();
    }
    if(ImGui::IsKeyPressed(ImGuiKey_Q) & ImGui::IsWindowFocused()){
        *window_5 = false;
    }

    Window5::window5_plots(result_for_table_tab2);

    ImGui::End();
    ImPlot::DestroyContext();

}

void Window5::window5_plots(pqxx::result* result_for_table_tab2) {
    
    if(!result_for_table_tab2->empty()){

        //store result table in 2dvector
        std::vector<std::vector<int>> vector2d;
        for (auto const &row : *result_for_table_tab2){
            std::vector<int> vector1d;
            for(auto const &field : row){
                vector1d.push_back(std::atoi(field.c_str()));    
            }
            vector2d.push_back(vector1d);
        }

        //get column names and its value, so std::unorderedmap<column name, value>
        std::unordered_map<std::string, std::vector<int>> column_names;
        if(column_names.size() < result_for_table_tab2->columns()) {
            for (int i = 0; i < result_for_table_tab2->columns(); ++i){ //for every column //max 8
                std::vector<int> column_values;
                for (int j = 0; j < result_for_table_tab2->size(); ++j) { //for every row //max 14
                    column_values.push_back(vector2d[j][i]);
                }
                column_names.insert(std::make_pair(result_for_table_tab2->column_name(i), column_values));
            }
        }
                                            /**
                                            //print
                                            if(!column_names.empty()) {
                                                for(auto each : column_names) {
                                                    std::vector<int> eachvector = each.second;
                                                    for(int i = 0; i < eachvector.size(); ++i) {
                                                        std::cout << each.first << ", " << eachvector[i] << ", ";
                                                    }
                                                    std::cout << std::endl;
                                                }
                                            }
                                            */

        //unordermap<colname, sumofcol>
        std::unordered_map<std::string, int> sum_of_col;
        if(!column_names.empty()){
            for(auto each : column_names){
                int sum = 0;
                for (int i=0; i < each.second.size(); ++i){
                    sum = sum + each.second.at(i);
                }
                sum_of_col.insert(std::make_pair(each.first, sum));
            }
        }

                                        /**
                                        //print
                                        if(!sum_of_col.empty()){
                                            for(auto each : sum_of_col){
                                                std::cout << each.first << ", " << each.second << std::endl;
                                            }
                                        }
                                        */

        //data structure to store correlation between columns
        std::unordered_map<std::string, std::unordered_map<std::string, float>> corr_data;

        if(!column_names.empty()) {
            for (std::unordered_map<std::string, std::vector<int>>::const_iterator ite = column_names.begin(); ite != column_names.end(); ++ite) {
                
                std::unordered_map<std::string, float> individual_unorderedmap;
                std::vector<int> vector_one = ite->second;
                for (auto each : column_names){
                    std::vector<int> vector_two = each.second;
                    float corr = Window5::calculate_correlation(&vector_one, &vector_two);
                    individual_unorderedmap.insert(std::make_pair(each.first, corr));
                }
                corr_data.insert(std::make_pair(ite->first, individual_unorderedmap));
            }

                                            /**
                                            //print
                                            if(!corr_data.empty()){
                                                for(auto each1 : corr_data){
                                                    std::unordered_map<std::string, float> individual_um = each1.second;
                                                    for(auto each2 : individual_um){
                                                        std::cout << each1.first << ": " << each2.first << ", " << each2.second;
                                                    }
                                                    std::cout << std::endl;
                                                }
                                            }
                                            */
        }

        //corr_data's individual um's to vector2d
        std::vector<std::vector<float>> vector2d_corr_data;
        if(!corr_data.empty()){
            for(auto each1 : corr_data){
                std::unordered_map<std::string, float> individual_um = each1.second;
                std::vector<float> vector1d_corr_data;
                for(auto each2 : individual_um) {
                    if(each2.second != each2.second) {
                        vector1d_corr_data.push_back(0);
                    }
                    else{
                        vector1d_corr_data.push_back(each2.second);
                    }
                }
                vector2d_corr_data.push_back(vector1d_corr_data);
            }
        }

        //vector2d of corr_data to array2d
        float array2d_corr_data[result_for_table_tab2->columns()][result_for_table_tab2->columns()];
        if(!corr_data.empty()){
            for(int i = 0; i < result_for_table_tab2->columns(); ++i) {
                for(int j = 0; j < result_for_table_tab2->columns(); j++){
                    //array2d[i][j] = vector2d[i][j];
                    //std::cout << i << ", " << j << std::endl;
                    if(vector2d_corr_data[i][j] == 0.0){
                        array2d_corr_data[i][j] = float(std::floor(vector2d_corr_data[i][j] * 100.) / 100.);
                    }
                    else {
                        array2d_corr_data[i][j] = float(std::floor(vector2d_corr_data[i][j] * 100.) / 100.);
                    }
                }
            }
                                                    /**
                                                    //print
                                                    int size = sizeof(array2d_corr_data) / sizeof(*array2d_corr_data);
                                                    for(int i=0; i < size; ++i){
                                                        for (int j=0; j < size; ++j){
                                                            std::cout<< array2d_corr_data[i][j] << " ";
                                                        }
                                                        std::cout << std::endl;
                                                    }
                                                    */
        }


        static float scale_min1 = 0;
        static float scale_max1 = 6.3f;

        std::vector<std::string> colnames_vector;
        for(auto each : column_names){
            colnames_vector.emplace(colnames_vector.begin(), each.first.c_str());
        }
        const char* xlabels1[result_for_table_tab2->columns()];
        const char* ylabels1[result_for_table_tab2->columns()];
        for (int i = 0; i < colnames_vector.size(); ++i){
            xlabels1[i] = colnames_vector[i].c_str();
            ylabels1[i] = colnames_vector[i].c_str();
        }

/**
        static ImPlotColormap map1 = ImPlotColormap_Viridis;
        if (ImPlot::ColormapButton(ImPlot::GetColormapName(map1),ImVec2(225,0),map1)) {
            map1 = (map1 + 1) % ImPlot::GetColormapCount();
            // We bust the color cache of our plots so that item colors will
            // resample the new colormap in the event that they have already
            // been created. See documentation in implot.h.
            ImPlot::BustColorCache("##Heatmap2");
        }
*/
        static ImPlotHeatmapFlags hm_flags1 = 0;

        static ImPlotAxisFlags axes_flags1 = ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks;

        static ImPlotColormap map1 = ImPlotColormap_Plasma;
        ImPlot::PushColormap(map1);

        if (ImPlot::BeginPlot("##Heatmap1",ImVec2(425,425),ImPlotFlags_NoLegend|ImPlotFlags_NoMouseText)) {
            ImPlot::SetupAxes(nullptr, nullptr, axes_flags1, axes_flags1);
            ImPlot::SetupAxisTicks(ImAxis_X1,0 + 1.0/14.0, 1 - 1.0/14.0, result_for_table_tab2->columns(), xlabels1);
            ImPlot::SetupAxisTicks(ImAxis_Y1,1 - 1.0/14.0, 0 + 1.0/14.0, result_for_table_tab2->columns(), ylabels1);
            ImPlot::PlotHeatmap("heat",array2d_corr_data[0],result_for_table_tab2->columns(),result_for_table_tab2->columns(),scale_min1,scale_max1,nullptr,ImPlotPoint(0,0),ImPlotPoint(1,1),hm_flags1);
            ImPlot::EndPlot();
        }
    }
 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////HELPER FUNCTIONS/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Window5::calculate_correlation(std::vector<int>* vector_one, std::vector<int>* vector_two){

    //get vector_one_avg
    float vector_one_avg = 0;
    float vector_one_sum = 0;
    float vector_one_size = vector_one->size();
    for(auto each : *vector_one){
        vector_one_sum = vector_one_sum + each;
    }
    vector_one_avg = vector_one_sum / vector_one_size;

    //get vector_two_avg
    float vector_two_avg = 0;
    float vector_two_sum = 0;
    float vector_two_size = vector_two->size();
    for(auto each : *vector_two){
        vector_two_sum = vector_two_sum + each;
    }
    vector_two_avg = vector_two_sum / vector_two_size;

    //create sum_xy and get its average
    std::vector<int> sum_xy_vector;
    for(int i = 0; i < vector_one_size; ++i){
        sum_xy_vector.push_back(vector_one->at(i) * vector_two->at(i));
    }
    float sum_xy_avg = 0;
    float sum_xy_sum = 0;
    float sum_xy_size = sum_xy_vector.size();
    for(auto each : sum_xy_vector){
        sum_xy_sum = sum_xy_sum + each;
    }
    sum_xy_avg = sum_xy_sum / sum_xy_size;

    //create squaresum_x and get its average
    std::vector<int> squaresum_x_vector;
    for(int i = 0; i < vector_one_size; ++i) {
        squaresum_x_vector.push_back(vector_one->at(i) * vector_one->at(i));
    }
    float squaresum_x_avg = 0;
    float squaresum_x_sum = 0;
    float squaresum_x_size = squaresum_x_vector.size();
    for(auto each : squaresum_x_vector) {
        squaresum_x_sum = squaresum_x_sum + each;
    }
    squaresum_x_avg = squaresum_x_sum / squaresum_x_size;

    //create squaresum_y and get its average
    std::vector<int> squaresum_y_vector;
    for(int i = 0; i < vector_one_size; ++i) {
        squaresum_y_vector.push_back(vector_two->at(i) * vector_two->at(i));
    }
    float squaresum_y_avg = 0;
    float squaresum_y_sum = 0;
    float squaresum_y_size = squaresum_y_vector.size();
    for(auto each : squaresum_y_vector) {
        squaresum_y_sum = squaresum_y_sum + each;
    }
    squaresum_y_avg = squaresum_y_sum / squaresum_y_size;

    int sum_X = 0, sum_Y = 0, sum_XY = 0;
    int squareSum_X = 0, squareSum_Y = 0;

    for(int i = 0; i < vector_one_size; ++i){
        double value_one = vector_one->at(i);
        double value_two = vector_two->at(i);

        sum_X = sum_X + value_one;
        sum_Y = sum_Y + value_two;
        sum_XY = sum_XY + value_one * value_two;
        squareSum_X = squareSum_X + value_one * value_one;
        squareSum_Y = squareSum_Y + value_two * value_two;
    }

    unsigned int numerator = ((vector_one_size * sum_XY) - (sum_X * sum_Y));
    double denominator_left = sqrt((vector_one_size * squareSum_X) - (sum_X * sum_X));
    double denominator_right = sqrt((vector_one_size * squareSum_Y) - (sum_Y * sum_Y));

    unsigned int denominator_all = (denominator_left * denominator_right);

    float corr = float(numerator) / float(denominator_all);
    std::cout << vector_one_avg <<","<< vector_two_avg <<","<< sum_xy_avg <<"," << squaresum_x_avg <<"," <<squaresum_y_avg << std::endl;

    return corr;
}