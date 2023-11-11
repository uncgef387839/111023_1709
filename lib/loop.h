#include "display/display.h"
#include "simulation/simulation.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

class Loop {
public:
    Loop(unsigned int sim_fps, unsigned int max_sim_frame_per_render,
         std::shared_ptr<Simulation> sim,
         std::shared_ptr<SimulationDisplay> display)
        : _max_sim_frame_per_render{max_sim_frame_per_render},
          _frameStep{1000000000 / sim_fps}, _sim(sim), _display(display) {

        if (!_sim)
            throw std::runtime_error("null sim");
    };

    void RunLoop() {

        // Simulation Loop - Constant Sim Speed with Maximum Display FPS
        // source: https://dewitters.com/dewitters-gameloop/
        unsigned int simFramesSinceRender = 0;
        auto nextGameTime = _clock::now();

        while (!_display->isClosed()) {
            // while (simFramesSinceRender < 1) {

            simFramesSinceRender = 0;

            // Simulate until we are caught up with the current time
            while (_clock::now() > nextGameTime &&
                   simFramesSinceRender < _max_sim_frame_per_render) {
                _sim->step();
                nextGameTime += _frameStep;
                simFramesSinceRender += 1;
            }
            // std::cout << "Sim frames since render: " << simFramesSinceRender
            //           << std::endl;

            _display->Update(); // Render
        }
    };

private:
    using _clock = std::chrono::steady_clock;
    using _duration = std::chrono::steady_clock::duration;
    using _rep = std::chrono::steady_clock::rep;

    const unsigned int _max_sim_frame_per_render;

    std::chrono::nanoseconds _frameStep;

    std::shared_ptr<Simulation> _sim;
    std::shared_ptr<SimulationDisplay> _display;
};