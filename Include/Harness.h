#ifndef _H_HARNESS
#define _H_HARNESS

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>
#include <functional>
#include <cmath>

#include "KeyListener.h"
#include "Renderer.h"


namespace qaiser{

    // Window properties
    struct Window{
        uint16_t width, height;
        const char* title;
        float color[4]{};

        explicit Window(uint16_t w=640, uint16_t h=480, const char* title="empty",
                        float r=1.0f, float g=1.0f, float b =0.0f)
        : width(w), height(h), title(title){
            color[0] = r;
            color[1] = g;
            color[2] = b;
            color[3] = 1.0f;
        }
    };

    // OpenGL rendering harness
    class Harness{
        // Probably replace with dynamic container later
        int keys_to_poll[4] =
                {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D};
        int currentKey    = 0;
        int currentAction = 0;
    public:
        Harness() = default;
        virtual ~Harness();

        void setWindow(const Window& window); // Call before running otherwise defaults
        const Window& getWindow() const;

        void toggleEditing();
        const bool isEditing() const;

        void setKA(const int& key, const int& action);
        static void notifyListeners();

        virtual void run(Harness* h);
        virtual void startup();

        virtual void render(float currentTime);

        static glm::mat4 VP;

    private:
        Window w;

    protected:
        GLuint VAO;
        GLuint buf[1024];
        Renderer* R;
        GLFWwindow* window;

    public:
        // States
        bool editing = false; // Mesh edit mode

        // For cursor restoration
        double saved_XPOS = 0.0f;
        double saved_YPOS = 0.0f;
    };

    // GLFW callbacks
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
}

#endif