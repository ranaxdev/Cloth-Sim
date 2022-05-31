#ifndef _H_KEYLISTENER
#define _H_KEYLISTENER

#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

class KeyListener{
public:
    int keys[348] ={0};
    double xpos=0.0, ypos=0.0;
    bool editing = false;

    virtual void editToggled() =0;

    static std::vector<KeyListener*> listeners;
};

#endif