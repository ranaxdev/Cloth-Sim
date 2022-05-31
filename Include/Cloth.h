#ifndef _H_CLOTH
#define _H_CLOTH


#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

// Single node in cloth grid
struct Particle{
    float mass;
    glm::vec3 net_force;
    int row,col; // index in the 2D grid
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 normal;

    glm::vec3 original_pos; // to restore if required later
    int num_neighbours;
    unsigned long vertex_index;

};

struct Spring{
    Particle* p1, *p2;
    float rest;
};


class Cloth{
public:
    Cloth();
    explicit Cloth(const char* path);

    void update();
    void manual_grid_construct(int n);
    void auto_grid_construct(const char* path);
    void recalculate_render_data();
    void reload_data();
    void save(std::string location);
    static bool particle_sort(Particle* p1, Particle* p2);

    bool intersect(glm::vec3 point,  glm::vec3 plane_origin);
    bool intersect(glm::vec3 point, glm::vec3 sphere_center, float radius);

    std::vector<std::vector<Particle*>> grid; // for manual generation
    std::vector<std::vector<Particle*>> quads; // for auto generation (non-guaranteed quad order)
    std::vector<Particle*> particles; // store unique particles if you want to update them all at once
    std::vector<Particle*> corners; // particles making up the corners of the cloth

    std::vector<Spring*> springs; // particle pairs (stretch, in-plane, out-plane)
    std::vector<float> render_data; // data for GL rendering

    glm::vec4 sphere_props = glm::vec4(0.0f);
    bool fix_corners = false;
    bool enable_collisions = false;
    bool wind = false;
    float wind_amount = 0.0f;
    bool spin = false;
    bool colliding = false;

    std::string face_part; // for file writing (storing faces section as-is)

private:
    float delta = 0.001f;
    float angle = 0.0f;
};

#endif