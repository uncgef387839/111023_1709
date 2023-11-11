#ifndef SIMULATION_DISPLAY
#define SIMULATION_DISPLAY
class SimulationDisplay {
public:
    virtual void Update() = 0;
    virtual bool isClosed() = 0;
    virtual ~SimulationDisplay(){};
};

#endif
