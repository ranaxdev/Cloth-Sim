#ifndef _H_SHADER
#define _H_SHADER

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

class Shader{
public:
    Shader() = default;
    Shader(std::string&& vshader_path, std::string&& fshader_path);
    ~Shader();

    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;

    void bind() const;
    void unbind() const;
    int get_attrib_loc(const char* var) const;

    void setVec3(int loc, glm::vec3& v);
    void setVec3(const char* name, glm::vec3& v);
    void setVec4(int loc, glm::vec4& v);
    void setVec4(const char* name, glm::vec4& v);
    void setMat4(int loc, glm::mat4& m);
    void setMat4(const char* name, glm::mat4& m);
    void setInt(int loc, int i);

private:
    void compile_error_checking(GLuint shader);
    void link_error_checking(GLuint program);
};


#endif