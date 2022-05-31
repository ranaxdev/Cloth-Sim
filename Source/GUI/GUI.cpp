#include <algorithm>
#include <iostream>
#include "GUI.h"

bool GUI::state = true;
// Add list of window flags
void GUI::addFlags(std::vector<ImGuiWindowFlags_>&& flags) {
    for(auto f : flags){
        GUI::flag_list.push_back(f);
    }
    GUI::updateFlags();
}

// Remove list of window flags
void GUI::removeFlags(std::vector<ImGuiWindowFlags_>&& flags) {
    // Remove subset of flags (toRemove) from menu flag list
    std::sort(flags.begin(), flags.end());
    GUI::flag_list.erase(
            std::remove_if(GUI::flag_list.begin(), GUI::flag_list.end(),
                           [&](auto x){return std::binary_search(flags.begin(), flags.end(),x);})
                           , GUI::flag_list.end());

    GUI::updateFlags();
}


// Update window flags from flags in the flag list vector (call after changing flags)
void GUI::updateFlags() {
    GUI::flags = ImGuiWindowFlags_None;
    std::sort(GUI::flag_list.begin(), GUI::flag_list.end());

    for(auto& f : GUI::flag_list){
        GUI::flags = static_cast<ImGuiWindowFlags_>(GUI::flags | f);
    }
}

GUI::GUI() {
    style->Colors[ImGuiCol_TitleBgActive] = dblue;
    style->Colors[ImGuiCol_TitleBg] = dblue;
}

GUI::~GUI() = default;



