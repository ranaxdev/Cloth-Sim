#ifndef _H_RENDERER
#define _H_RENDERER

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "Cloth.h"
#include "Model.h"
#include "Menu.h"
#include "Globals.h"
#include "Shader.h"

class Renderer{
public:
    Renderer(GLuint& VAO, GLuint buf[]);
    ~Renderer() = default;

    void enableModel(Model& m);
    void enableCloth(std::vector<float>& data);
    Cloth* cloth;
    Model* sphere;
    Model* ground;


    void renderGUI(Menu& g);
    void renderCloth(Cloth& c);
    void renderModel(Model& m);

    unsigned int prepBuf(GLfloat data[], GLuint size);
    unsigned int prepBuf(GLushort data[], GLuint size);
    unsigned int prepBuf(std::vector<GLfloat>& data, bool big);
    unsigned int editBuf(std::vector<GLfloat>& data, GLuint i);

    void formatBuf(GLuint loc, GLint comps_per_elem, std::vector<int> attribs, Shader& s);

private:
    // Memory mgmt
    static int free_buf;
    static int free_bindpoint;
    GLuint VAO;
    GLuint* buf;
    GLuint cloth_buffer;

    // Shaders
    Shader shader_axis = Shader(SRC+"Shaders/vert.glsl", SRC+"Shaders/frag.glsl");

    // Colors
    glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 cyan = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

};
#endif