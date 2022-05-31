#include <string>
#include <iostream>
#include "Logger.h"
#include "Harness.h"
#include "Renderer.h"

// Static initializations
int Renderer::free_buf          = -1;
int Renderer::free_bindpoint    = -1;

Renderer::Renderer(GLuint &VAO, GLuint *buf)
: VAO(VAO), buf(buf) {
//    cloth = new Cloth();
//    enableCloth(cloth->render_data);

    sphere = new Model(std::string(SRC+"Models/sphere.obj").c_str(), "overt", "ofrag", std::make_pair(2,3));
    enableModel(*sphere);

    ground = new Model(std::string(SRC+"Models/ground.obj").c_str(), "o2vert", "o2frag", std::make_pair(4,5), glm::vec3(0.5f, 0.5f, 0.5f));
    ground->transformation = glm::translate(ground->transformation, glm::vec3(0.0f, -10.5f, 0.0f));
    enableModel(*ground);


}

/* ****************************************************************
 *                  RENDER-ABLES INITIALIZATIONS                  *
 ******************************************************************/

/*
 * Initialize cloth data
 */
void Renderer::enableCloth(std::vector<float>& data) {
    // Prepare buffer
    GLuint loc = prepBuf(data, true);
    cloth_buffer = loc;

    // Format data
    formatBuf(loc, 3, {0,1}, Renderer::shader_axis);
}

/*
 * Initialize model data
 */
void Renderer::enableModel(Model& m) {
    // Prepare buffer
    GLuint loc = prepBuf(m.model_data, true);

    // Format data
    formatBuf(loc, 3, {m.attribs.first, m.attribs.second}, *m.shader);
}


/* ****************************************************************
 *                        RENDERING ROUTINES                      *
 ******************************************************************/
void Renderer::renderCloth(Cloth& c){
    shader_axis.bind();
    shader_axis.setMat4(20, qaiser::Harness::VP);

    editBuf(c.render_data, cloth_buffer);
    glLineWidth(5.0f);

    // halve first to get rid of color data, /3 as each line element has 3 vertices
    int lines = (int)(((c.render_data.size()/2)/3));
    glDrawArrays(GL_LINES , 0, lines);

}

void Renderer::renderModel(Model &m) {
    m.shader->bind();
    m.shader->setMat4(21, m.transformation);
    m.shader->setMat4(20, qaiser::Harness::VP);

    int tris = (int)(m.model_data.size()/2);
    glDrawArrays(GL_TRIANGLES , 0, tris);

}


void Renderer::renderGUI(Menu &g) {
    g.update();

    // Load cloth file
    if(g.loaded){
        cloth = new Cloth(g.file_name.c_str());
        enableCloth(cloth->render_data);

        g.loaded = false;
        g.cloth_exists = true;
    }
    // Save file cloth
    if(g.saved && g.cloth_exists){
        cloth->save(g.save_file_name);
        g.saved = false;
    }

    // Rendering stuff
    if(g.cloth_exists){
        renderCloth(*cloth);
    }
    if(!g.scenario2){ // scenario2 is fixed corners stuff, dont need to render ground for that but otherwise, do.
        renderModel(*ground);
    }

    if(g.cloth_exists){

    // Sphere collision scenario
    if(g.scenario1){
        // Enable collisions
        cloth->enable_collisions = true;
        cloth->fix_corners = false;
        cloth->wind = false;
        // Render sphere and set its properties
        sphere->transformation = glm::mat4(1.0f);
        sphere->transformation = glm::translate(sphere->transformation, glm::vec3(g.sphere_pos_x, g.sphere_pos_y, g.sphere_pos_z));
        sphere->transformation = glm::scale(sphere->transformation, glm::vec3(g.sphere_radius, g.sphere_radius, g.sphere_radius));

        renderModel(*sphere);

        cloth->sphere_props = glm::vec4(g.sphere_pos_x, g.sphere_pos_y, g.sphere_pos_z, g.sphere_radius);


        // Start updating cloth forces
        if(g.start){
            for(int i=0; i<50; i++){
                cloth->update();
            }
            // update cloth render data after update
            cloth->recalculate_render_data();
        }
        // Reset grid
        if(g.reset){
            g.start = false;
            cloth->reload_data();
            cloth->update();
            g.reset = false;
        }

        // Sphere spin
        cloth->spin = g.spin;
    }


    // Free fall scenario
    if(g.scenario2){
        // Fix corners
        cloth->fix_corners = true;
        cloth->enable_collisions = false;
        cloth->spin = false;

        // Start updating cloth forces
        if(g.start){
            for(int i=0; i<50; i++){
                cloth->update();
            }
            // update cloth render data after update
            cloth->recalculate_render_data();
        }
        // Reset grid
        if(g.reset){
            g.start = false;
            cloth->reload_data();
            cloth->update();
            g.reset = false;
        }

        // Wind
        cloth->wind = g.wind;
        cloth->wind_amount = g.wind_amount;
    }

    }
}


/* ****************************************************************
 *                      BUFFER SETUP ROUTINES                     *
 ******************************************************************/
/*
 * @data - Array of float data
 * @size - Size of the array in bytes
 * Inits a new buffer and returns its index
 */
unsigned int Renderer::prepBuf(GLfloat *data, GLuint size) {
    free_buf++;
    glCreateBuffers(1, &buf[free_buf]);
    glNamedBufferStorage(buf[free_buf], size, data, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);

    return free_buf;
}
unsigned int Renderer::prepBuf(GLushort *data, GLuint size) {
    free_buf++;
    glCreateBuffers(1, &buf[free_buf]);
    glNamedBufferStorage(buf[free_buf], size, data, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);

    return free_buf;
}


/*
 * @data - List of float data
 * Inits a new buffer and returns its index
 */
unsigned int Renderer::prepBuf(std::vector<GLfloat>& data, bool big) {
    int size = (int) data.size();
    int dat_size = 4*size;

    // Either allocate a big buffer (for large meshes and such)
    // Or just as large as your data
    int to_allocate;
    if(big)
        to_allocate = ONE_MB;
    else
        to_allocate = dat_size;


    free_buf++;
    glCreateBuffers(1, &buf[free_buf]);
    glNamedBufferStorage(buf[free_buf], to_allocate, nullptr, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT); // 1mb buffer

    float* ptr = (float*) glMapNamedBufferRange(buf[free_buf], 0, dat_size, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);
    for(int i=0; i<size; i++){
        ptr[i] = data[i];
    }
    glUnmapNamedBuffer(buf[free_buf]);

    return free_buf;
}

/*
 * @buf - Buffer to edit
 * Edits buffer data
 */
unsigned int Renderer::editBuf(std::vector<GLfloat>& data, GLuint i) {
    int size = (int) data.size();
    int dat_size = 4*size;

    // Buffer overflowed
    if(dat_size >= ONE_MB){
        Logger::log(ERROR, "Buffer overflowed, buffer ID: "+ std::to_string(i), __FILENAME__);
    }

    float* ptr = (float*) glMapNamedBufferRange(buf[i], 0, dat_size, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT);
    for(int x=0; x<size; x++){
        ptr[x] = data[x];
    }
    glUnmapNamedBuffer(buf[i]);

    return i;
}


/*
 * @loc                     - Active buffer location
 * @comps_per_elem          - Number of components in an element
 * @names                   - List of attribute names used in GLSL
 * @s                       - Shader to get the attribute names from
 *
 * Formats the buffer for the VAO
 */

void Renderer::formatBuf(GLuint loc, GLint comps_per_elem, std::vector<int> attribs, Shader& s) {
    s.bind();
    free_bindpoint++;
    auto num_attribs = attribs.size();

    for(int i=0; i < num_attribs; i++){

        glVertexArrayAttribFormat(VAO, attribs[i], comps_per_elem, GL_FLOAT, GL_FALSE, (i*comps_per_elem)*sizeof(float));
        glVertexArrayAttribBinding(VAO, attribs[i], free_bindpoint);
        glEnableVertexArrayAttrib(VAO, attribs[i]);
    }

    glVertexArrayVertexBuffer(VAO, free_bindpoint, buf[loc], 0, (num_attribs*comps_per_elem)*sizeof(float));
}






