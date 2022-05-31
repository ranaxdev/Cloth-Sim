#include "Menu.h"
#include <iostream>

Menu::Menu() {
    Menu::size = ImVec2(360.0f, 480.0f);
    Menu::pos = ImVec2(SCREEN_W-size.x, 0.0f);


    // Init flags
    Menu::flag_list = {
            ImGuiWindowFlags_MenuBar
    };
    updateFlags();

}

void Menu::update() {

    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("Cloth Menu", &GUI::state, flags);
    ImGui::Text("Select options below \n\n");

    // Load a cloth obj file
    if(ImGui::Button("Load Cloth")){
        open_file_inst.OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", ".");
    }
    if (open_file_inst.Display("ChooseFileDlgKey"))
    {
        // OK clicked on file
        if (open_file_inst.IsOk())
        {
            file_name = open_file_inst.GetFilePathName(); // store file name clicked on
            loaded = true;
            cloth_exists = true;
        }

        // close
        open_file_inst.Close();
    }

    // Write a cloth obj file
    if(ImGui::Button("Write Cloth")){
        if(cloth_exists){
            save_file_inst.OpenDialog("ChooseFileDlgKey2", "Save File", ".obj", ".");
        }
    }
    if(save_file_inst.Display("ChooseFileDlgKey2")){
        // OK clicked on file
        if(save_file_inst.IsOk()){
            save_file_name = save_file_inst.GetFilePathName();
            saved = true;
        }
        // Close
        save_file_inst.Close();
    }


    // Scenario 1 - collision with sphere
    if(ImGui::Button("Collision")){
        scenario1 = true;
        scenario2 = false;
        reset = true;
        wind = false;
    }
    if(scenario1){
        // set sphere properties
        ImGui::TextColored(lblue, "================");
        ImGui::SliderFloat("Pos X", &sphere_pos_x, -20.0f, 20.0f);
        ImGui::SliderFloat("Pos Y", &sphere_pos_y, -20.0f, 20.0f);
        ImGui::SliderFloat("Pos Z", &sphere_pos_z, -20.0f, 20.0f);
        ImGui::SliderFloat("Radius", &sphere_radius, 0.5f, 5.0f);

        // start the simulation
        if(ImGui::Button("Start")){
            start = true;
        }
        // reset the simulation
        ImGui::SameLine();
        if(ImGui::Button("Reset")){
            reset = true;
        }
        // Enable sphere spin
        if(ImGui::Button("Spin")){
            spin = true;
        }
    }


    // Scenario 2 - two corners fixed with free fall
    if(ImGui::Button("Free Fall")){
        scenario1 = false;
        scenario2 = true;
        reset = true;
    }
    if(scenario2){
        // start the simulation
        if(ImGui::Button("Start")){
            start = true;
        }
        // reset the simulation
        ImGui::SameLine();
        if(ImGui::Button("Reset")){
            reset = true;
        }

        // Enable wind
        if(ImGui::Button("Wind")){
            wind = true;
        }
        if(wind){
            ImGui::SliderFloat("Amount (%)", &wind_amount, 0.0f, 100.0f);
        }
    }


    ImGui::End();

}
