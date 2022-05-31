#include "Cloth.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>


Cloth::Cloth() {
    manual_grid_construct(10);
}

Cloth::Cloth(const char *path) {
    // Clear previously stored data
    for(auto& p : particles){
        delete p;
    }
    for(auto& s : springs){
        delete s;
    }
    for(auto& q : quads){
        q.clear();
    }
    quads.clear();
    particles.clear();
    springs.clear();
    render_data.clear();


    auto_grid_construct(path);
}

// Wavefront OBJ cloth file grid/spring construction
// This will import the cloth grid which is made up of quads and make springs out of the edges
// Also added 2 in-plane springs between each quad
void Cloth::auto_grid_construct(const char* path){
    float x, y, z;
    std::ifstream filestream(path, std::ios::in);
    std::string line = "";
    std::stringstream ss;
    std::vector<float> vertices;

    // Parse the cloth file
    while (!filestream.eof()) {
        std::getline(filestream, line);

        // Parse vertices
        if (line.compare(0, 2, "v ") == 0) {
            std::stringstream ss(line.erase(0, 1));
            ss >> x;
            ss >> y;
            ss >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }

        // Parse quad faces
        if (line.compare(0, 2, "f ") == 0) {
            face_part += (line + "\n");
            std::string node, vert;
            std::stringstream ss(line.erase(0, 2));

            std::vector<Particle*> current_quad; // consists of 4 vertices
            current_quad.clear();

            // Parse each vertex in the quad
            for(int i=0; i<4; i++){
                std::getline(ss, node, ' ');
                std::stringstream node_ss(node);
                std::getline(node_ss, vert, '/');

                // get indices for vertices (to access them from the vectors we stored them in)
                std::size_t vert_index = ((std::size_t)(std::stoi(vert)) - 1) * 3;

                // get one of the 4 vertices in the quad
                glm::vec3 pos = glm::vec3(vertices[vert_index + 0],vertices[vert_index + 1],vertices[vert_index + 2]);

                // storing quad structs (made up of 4 particles) to construct springs later
                // store unique particle (make sure the vertex wasn't previously stored)
                bool exists = false;
                int index = -1;
                for(int j=0; j <particles.size(); j++){
                    if(pos == particles[j]->position){
                        exists = true;
                        index = j;
                        break;
                    }
                }
                if(exists){
                    particles[index]->num_neighbours++; // count neighbours
                    current_quad.push_back(particles[index]);
                }
                else{ // Particle wasn't previously instantiated, create it
                    Particle* p = new Particle;
                    p->mass = 0.5f;
                    p->net_force = glm::vec3(0.0f);
                    p->velocity = glm::vec3(0.0f);
                    p->normal = glm::vec3(0.0f, 1.0f, 0.0f);
                    p->position = pos;
                    p->original_pos = pos;
                    p->num_neighbours = 0;
                    p->vertex_index = vert_index;

                    particles.push_back(p);
                    current_quad.push_back(p);
                }

            }

            quads.push_back(current_quad); // store quad
        }
    }

    // Find corner particles (to fix later)
    // They should only have 2 neighbours, the rest would have more (thinking about the grid)
    for(auto& p : particles){
        if(p->num_neighbours == 0)
            corners.push_back(p);
    }

    // Construct springs from quads
    for(auto& quad : quads){
        // Regular edges (stretch)
        auto* s1 = new Spring{quad[0], quad[1], glm::distance(quad[0]->position, quad[1]->position)};
        auto* s2 = new Spring{quad[1], quad[2], glm::distance(quad[1]->position, quad[2]->position)};
        auto* s3 = new Spring{quad[2], quad[3], glm::distance(quad[2]->position, quad[3]->position)};
        auto* s4 = new Spring{quad[3], quad[0], glm::distance(quad[3]->position, quad[0]->position)};

        // In-plane edges (diagonals)
        auto* s5 = new Spring{quad[0], quad[2], glm::distance(quad[0]->position, quad[2]->position)};
        auto* s6 = new Spring{quad[1], quad[3], glm::distance(quad[1]->position, quad[3]->position)};

        springs.push_back(s1);
        springs.push_back(s2);
        springs.push_back(s3);
        springs.push_back(s4);
        springs.push_back(s5);
        springs.push_back(s6);


    }

    // initialize render data
    recalculate_render_data();

}



// Construct an n*n grid for the cloth, creating links for the
// direct neighbours & in/out skew neighbours
void Cloth::manual_grid_construct(int n) {
    grid.resize(n, std::vector<Particle*>(n));

    // First pass, fill grid with particles
    for(int i=0; i < n; i++){
        for(int j=0; j< n; j++){
            Particle* p = new Particle;
            p->mass = 0.5f;
            p->net_force = glm::vec3(0.0f);
            p->row = i; p->col = j;
            p->velocity = glm::vec3(0.0f);
            p->normal = glm::vec3(0.0f, 1.0f, 0.0f);
            grid[i][j] = p;

            particles.push_back(p);
        }
    }

    float temp_spacingX = -(float)n;
    float temp_spacingZ = -(float)n;
    // Second pass, assign neighbours
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            // direct neighbours (up, down, left, right)
            if(i-1 >= 0){ // up
                auto* s = new Spring{grid[i][j], grid[i-1][j], 1.0f};
                springs.push_back(s);
            }
            if(i+1 < n){ // down
                auto* s = new Spring{grid[i][j], grid[i+1][j], 1.0f};
                springs.push_back(s);
            }
            if(j-1 >=0){ // left
                auto* s = new Spring{grid[i][j], grid[i][j-1], 1.0f};
                springs.push_back(s);
            }
            if(j+1 < n){ // right
                auto* s = new Spring{grid[i][j], grid[i][j+1], 1.0f};
                springs.push_back(s);
            }

            // in-plane skew neighbours (diagonals)
            if(i-1 >=0 && j-1 >=0){ // top-left
                auto* s = new Spring{grid[i][j], grid[i-1][j-1], (float)sqrt(2.0)};
                springs.push_back(s);

            }
            if(i-1 >=0 && j+1 <n){ // top-right
                auto* s = new Spring{grid[i][j], grid[i-1][j+1], (float)sqrt(2.0)};
                springs.push_back(s);
            }
            if(i+1 < n && j-1 >=0){ // bottom-left
                auto* s = new Spring{grid[i][j], grid[i+1][j-1], (float)sqrt(2.0)};
                springs.push_back(s);
            }
            if(i+1 <n && j+1 < n){ // bottom-right
                auto* s = new Spring{grid[i][j], grid[i+1][j+1], (float)sqrt(2.0)};
                springs.push_back(s);
            }

            // out-plane skew neighbours (up+1, down+1, left+1, right+1)
            if(i-2 >= 0){ // up+1
                auto* s = new Spring{grid[i][j], grid[i-2][j], 2.0f};
                springs.push_back(s);
            }
            if(i+2 < n){ // down+1
                auto* s = new Spring{grid[i][j], grid[i+2][j], 2.0f};
                springs.push_back(s);
            }
            if(j-2 >=0){ // left+1
                auto* s = new Spring{grid[i][j], grid[i][j-2], 2.0f};
                springs.push_back(s);
            }
            if(j+2 < n){ // right+1
                auto* s = new Spring{grid[i][j], grid[i][j+2], 2.0f};
                springs.push_back(s);
            }

            grid[i][j]->position.x = temp_spacingX;
            grid[i][j]->position.z = temp_spacingZ;
            grid[i][j]->position.y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.0));;

            temp_spacingX += 1.0f;
        }
        temp_spacingX = -(float)n;
        temp_spacingZ+=1.0f;
    }

    // initialize render data
    recalculate_render_data();

}

void Cloth::update() {
    // Reset net force
    for(auto& p : particles){
        p->net_force = glm::vec3(0.0f);
    }

    // Apply spring forces
    for(auto& s: springs){
        auto len = glm::length(s->p2->position - s->p1->position);
        auto diff_vel = s->p2->velocity - s->p1->velocity;

        glm::vec3 dir;
        dir = glm::normalize(s->p2->position - s->p1->position);


        float k = 10.0f;
        glm::vec3 F = ((-k * (len - s->rest)) - 5.0f*(glm::dot(diff_vel,dir)))  * dir;

        s->p1->net_force += (-F * s->p1->mass);
        s->p2->net_force += (F * s->p2->mass);


        // fix points
        if(fix_corners){
            corners[0]->net_force = glm::vec3(0.0f);
            corners[1]->net_force = glm::vec3(0.0f);
        }

    }


    for(auto& p : particles){
        // gravitational force (scaled down)
        p->net_force.y += (-9.81f*0.1f * p->mass);

        // Add wind force
        if(wind){
            p->net_force.z -= (5.0f*(wind_amount/100.0f) * p->mass);
        }

        // fix corners
        if(fix_corners){
            corners[0]->net_force = glm::vec3(0.0f);
            corners[1]->net_force = glm::vec3(0.0f);
        }


        p->velocity += ((p->net_force*delta)/p->mass);
        angle += (delta* 0.01f);

        // sphere-cloth collision
        if(enable_collisions){
            if(intersect(p->position, glm::vec3(sphere_props.x, sphere_props.y, sphere_props.z), sphere_props.w + 0.5f)){
                // calculate rebound velocity
                glm::vec3 sphere_center = glm::vec3(sphere_props);
                glm::vec3 temp = (sphere_center - p->velocity);
                temp = glm::normalize(temp);
                temp = temp * (sphere_props.w + 0.5f);

                p->velocity = (delta * -(sphere_center + temp));

                // Apply spin when colliding with sphere (emulating frictional force)
                if(spin){
                    glm::mat4 trans = glm::mat4(1.0f);
                    glm::mat4 translate = glm::translate(glm::mat4(1.0f), sphere_center);
                    glm::mat4 inv_translate = glm::inverse(translate);
                    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(0.001f*angle), glm::vec3(0.0f, 1.0f, 0.0f));
                    trans = translate * rot * inv_translate;

                    glm::vec4 temp2 = glm::vec4(p->position.x, p->position.y, p->position.z, 0.0f);
                    temp2 = temp2 * trans;
                    p->position = glm::vec3(temp2.x, temp2.y, temp2.z);
                }
            }

            // plane-cloth collision
            if(intersect(p->position, glm::vec3(0.0f, -10.0f, 0.0f))){
                glm::vec3 plane_normal = glm::vec3(0.0f, 1.0f, 0.0f);
                glm::vec3 temp = plane_normal - p->velocity;
                temp = glm::normalize(temp);
                p->velocity = (delta * -(plane_normal + temp));
            }
        }

        // calculate new position
        p->position += (delta * p->velocity);
    }

}

// Recalculates vertices for rendering, using updated positions
void Cloth::recalculate_render_data() {
    render_data.clear();
     // Set the render data
     for(auto& s : springs){
         render_data.push_back(s->p1->position.x);
         render_data.push_back(s->p1->position.y);
         render_data.push_back(s->p1->position.z);

         render_data.push_back(0.0f);
         render_data.push_back(1.0f);
         render_data.push_back(0.0f);

         render_data.push_back(s->p2->position.x);
         render_data.push_back(s->p2->position.y);
         render_data.push_back(s->p2->position.z);

         render_data.push_back(0.0f);
         render_data.push_back(1.0f);
         render_data.push_back(0.0f);

     }

}

// Restore rendering data to original flat cloth state
void Cloth::reload_data() {
    // Reset particles in the grid
    for(auto& p : particles){
        p->position = p->original_pos;
    }
    recalculate_render_data();
}





bool Cloth::intersect(glm::vec3 point, glm::vec3 plane_origin) {
    double D = glm::dot(plane_origin, glm::vec3(0.0f, 1.0f, 0.0f));

    double numer = glm::dot(glm::vec3(0.0f, -1.0f, 0.0f), point)  +D ;
    double denom = glm::dot(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    if(-(numer/denom) >= 0.0)
        return true;

    return false;
}

bool Cloth::intersect(glm::vec3 point, glm::vec3 sphere_center, float radius) {
    float distance = glm::distance(point, sphere_center);
    return distance < radius;
}

// Sort particles by their vertex index
bool Cloth::particle_sort(Particle* p1, Particle* p2) {
    if(p1->vertex_index != p2->vertex_index){
        return p1->vertex_index < p2->vertex_index;
    }
    return false;
}


// Save cloth file as obj with the current state of the vertices
void Cloth::save(std::string location) {
    std::string obj_file;
    // sort particles in order of vertex indices
    std::sort(particles.begin(), particles.end(), &(Cloth::particle_sort));

    // write updated vertex positions to file
    for(auto& p: particles){
        obj_file.append("v " + std::to_string(p->position.x) + " " + std::to_string(p->position.y) +" " + std::to_string(p->position.z) + "\n");
    }
    // write faces in
    obj_file.append(face_part);

    // Save the file to the selected save location and file path
    std::ofstream savefile(location);
    savefile << obj_file;
    savefile.close();

}



