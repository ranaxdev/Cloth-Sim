#include <iostream>
#include "Camera.h"

Camera::Camera(){
    KeyListener::listeners.push_back(this); // Register this as KL
}

void Camera::editToggled() {}

glm::mat4 &Camera::calc_VP(float delta) {
    if(!editing){ // Camera updates in normal mode, stays still in edit mode
        Camera::velocity = 7.0f * delta;
        // Update Movement
        if(keys[FWD])
            cam_pos += velocity * cam_front;
        if(keys[BCK])
            cam_pos -= velocity * cam_front;
        if(keys[LFT])
            cam_pos -= glm::normalize(glm::cross(cam_front, cam_up))*velocity;
        if(keys[RGT])
            cam_pos += glm::normalize(glm::cross(cam_front, cam_up))*velocity;

        // Update Rotation
        xoff = xpos - lastX;
        yoff = lastY - ypos;
        lastX = xpos;
        lastY = ypos;
        xoff *= sens;
        yoff *= sens;
        yaw += xoff;
        pitch += yoff;
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;


        cam_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cam_front.y = sin(glm::radians(pitch));
        cam_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        Camera::view_m = glm::lookAt(cam_pos, cam_pos+cam_front, cam_up);
        Camera::vp_m = Camera::proj_m * Camera::view_m;

    }
    return Camera::vp_m;
}

const glm::vec3 &Camera::getCamPos() const {
    return Camera::cam_pos;
}

