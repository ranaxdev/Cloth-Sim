#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Menu.h"
#include "Harness.h"
#include "Camera.h"
#include "Model.h"
#include "Cloth.h"

using namespace qaiser;
class App : public qaiser::Harness{
public:
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::shared_ptr<Menu> menu;

    void startup() override {
        menu = std::make_shared<Menu>();
    };
    

    void render(float delta) override {
        VP = camera->calc_VP(delta);

        R->renderGUI(*menu);
    }
};


#if !DEBUG
int main(){
    qaiser::Window window = qaiser::Window(1920, 1080, "Cloth Simulator", 1.0f, 1.0f, 1.0f);
    App* a = new App;
    a->setWindow(window);
    a->run(a);

    delete a;
    
    return 0;
}


#else
// empty
#endif