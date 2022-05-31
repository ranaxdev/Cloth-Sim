#ifndef _H_MENU
#define _H_MENU

#include "GUI.h"
#include "ImGuiFileDialog.h"

class Menu : public GUI{
public:
    Menu();

    ImVec2 size;
    ImVec2 pos;
    void update() override;

    // simulations
    bool cloth_exists = false;
    bool start = false;
    bool reset = false;
    // sphere collision
    bool scenario1 = false;
    float sphere_pos_x = 0.0f;
    float sphere_pos_y = 0.0f;
    float sphere_pos_z = 0.0f;
    float sphere_radius = 1.0f;
    bool spin = false;
    // free fall
    bool scenario2 = false;
    bool wind = false;
    float wind_amount = 0.0f;


    // file management
    ImGuiFileDialog open_file_inst;
    ImGuiFileDialog save_file_inst;
    std::string file_name;
    std::string save_file_name;
    bool loaded;
    bool saved;

};

#endif