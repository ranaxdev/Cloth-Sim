#include <iostream>
#include <algorithm>

#include "Globals.h"
#include "Logger.h"
#include "Harness.h"

/**************************************************************************
 *                              HARNESS
 ************************************************************************* */
glm::mat4 qaiser::Harness::VP = glm::mat4(1.0f);
float delta;
qaiser::Harness::~Harness() {
    // Dump log file
    Logger::dump();
}

void qaiser::Harness::run(qaiser::Harness* h) {
    // GLFW init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFW window
    window = glfwCreateWindow(w.width, w.height, w.title, nullptr, nullptr);
    if(window == nullptr){
        Logger::log(ERROR, "Could not create GLFW window", __FILENAME__);
        glfwTerminate();
    }
    // Window settings
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, (void*)(this)); // Pointer to app window that implements this harness
    // GLFW settings
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Callbacks
    glfwSetKeyCallback(window, qaiser::key_callback);

    // Load GLAD
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        Logger::log(ERROR, "Could not load GLAD", __FILENAME__);
        glfwTerminate();
    }

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glViewport(0,0,1920,1080);

    /* Application Initialization */
    glCreateVertexArrays(1, &VAO); // Init VAO
    Harness::R = new Renderer(Harness::VAO, Harness::buf); // Renderer instance
    IMGUI_CHECKVERSION();

    // imGUI initialization
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF((FONT_ROBOTO).c_str(), 20);
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");

    // App instance init
    startup();

    // Activate VAO
    glBindVertexArray(VAO);


    // Application Rendering
    const GLfloat screen_color[4] = {w.color[0], w.color[1], w.color[2], w.color[3]};
    const float one = 1.0f;
    float last = 0.0f;
    while(!glfwWindowShouldClose(window)){
        float currentTime = (float)glfwGetTime();
        delta = currentTime - last;

        if(delta >= INTERVAL_MAX){
            glClearBufferfv(GL_COLOR, 0, screen_color);
            glClearBufferfv(GL_DEPTH, 0, &one);

            // Update observers
            for(auto& o : KeyListener::listeners){
                // NOTE: for key callback (NOT poll) => o->keys[currentKey] = currentAction
                // Poll tracked keys for presses & mouse positions
                for(auto& k : keys_to_poll){
                    if(glfwGetKey(window, k) == GLFW_PRESS)
                        o->keys[k] = 1;
                    else
                        o->keys[k] = 0;
                }
                // Send current cursor position
                if(!editing)
                    glfwGetCursorPos(window, &o->xpos, &o->ypos);

                // Send editing state
                o->editing = Harness::editing;
            }

            // GUI Rendering frame initialization
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Render application
            render(delta);

            // Render GUI
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            glfwSwapBuffers(window);
            glfwPollEvents();

            last = currentTime;
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void qaiser::Harness::startup() {}

void qaiser::Harness::render(float currentTime) {}

void qaiser::Harness::setWindow(const qaiser::Window &window) {
    qaiser::Harness::w = window;
}

const qaiser::Window& qaiser::Harness::getWindow() const {
    return Harness::w;
}

void qaiser::Harness::setKA(const int &key, const int &action) {
    Harness::currentKey = key;
    Harness::currentAction = action;
}

void qaiser::Harness::toggleEditing() {
    Harness::editing = !Harness::editing;
}

const bool qaiser::Harness::isEditing() const {
    return Harness::editing;
}

void qaiser::Harness::notifyListeners() {
    for(auto& o : KeyListener::listeners){
        o->editToggled();
    }
}

/**************************************************************************
 *                          QAISER ROUTINES GENERAL
 ************************************************************************* */
void qaiser::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Retrieve harness of current window
    Harness* instance = (Harness*)glfwGetWindowUserPointer(window);

    // Send key info to application that implements the harness
    if(instance){
        instance->setKA(key, action);
    }

    // Shutdown
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);


    // Toggle Edit Mode
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        instance->toggleEditing();
        instance->notifyListeners();

        /*
         * Save last cursor position when entering edit mode
         * Reload cursor position when exiting edit mode
         */
        if(instance->isEditing()){
            glfwGetCursorPos(window, &instance->saved_XPOS, &instance->saved_YPOS);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, instance->saved_XPOS, instance->saved_YPOS);
        }
    }
}