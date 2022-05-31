#ifndef _H_MODEL
#define _H_MODEL

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Globals.h"
#include "Shader.h"


class Model{
public:
    explicit Model(const char* path, std::string vert, std::string frag, std::pair<int,int> attribs, glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f));

    std::vector<float> model_data;
    glm::mat4 transformation;

    Shader* shader;
    std::pair<int,int> attribs;
private:
    std::vector<float> vertices;

};

#endif