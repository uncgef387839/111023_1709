#ifndef WINDOW_CREATE_INFO
#define WINDOW_CREATE_INFO

#include <iostream>

struct WindowCreateInfo {
    // WindowCreateInfo() : height(800), width(1280), title("Simulation
    // Window"){};
    const unsigned int height = 800;
    const unsigned int width = 1280;
    const char *title = "Simulation Window";
    friend std::ostream &operator<<(std::ostream &os,
                                    const WindowCreateInfo &wci);
};

#endif