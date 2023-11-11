#include "lib/display/deligate.h"
#include "lib/display/display.h"
#include "lib/display/vk_display.h"
#include "lib/loop.h"
#include "lib/simulation/simulation.h"
#include "lib/trimesh.h"
#include <chrono>
#include <cstdio>
#include <iostream>
#include <vector>

#include <unistd.h>

int main(int argc, char *argv[]) {

    std::vector<Object> objects;

    ObjTriMesh square_mesh("data/square.obj");
    ObjTriMesh floor_mesh("data/floor.obj");

    objects.emplace_back(square_mesh, false, -1, 1.);
    objects.emplace_back(floor_mesh, true, -1, 1.);

    const int simFPS = 60;
    const int maxSimStepsPerFrame = 2;
    // const float simSlowdown = 1;
    const float simSlowdown = 1E-4;

    const WindowCreateInfo window;

    std::unique_ptr<DisplaySimulationDeligate> deligate(
        new DisplaySimulationDeligate(objects));

    std::shared_ptr<SimulationDisplay> display(
        dynamic_cast<SimulationDisplay *>(
            new VKSimulationDisplay(window, std::move(deligate))));

    std::shared_ptr<Simulation> sim(
        new Simulation(objects, 1. / simFPS * simSlowdown));
    // std::shared_ptr<Simulation> sim = nullptr;

    Loop loop(simFPS, maxSimStepsPerFrame, std::move(sim), display);
    loop.RunLoop();

    return 0;
}
