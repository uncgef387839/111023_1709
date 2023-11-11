#ifndef SIMULATION_H
#define SIMULATION_H

#include "lib/object.h"
#include <Eigen/Dense>
#include <memory>

class Simulation {
public:
    Simulation(std::vector<Object> &objects, float timestep_size);
    void step();
    void Print();
    static inline void calculateNormal(const Eigen::Vector3f &t1,
                                       const Eigen::Vector3f &t2,
                                       const Eigen::Vector3f &t3,
                                       Eigen::Vector3f &out);

    static inline float calculateCharacteristicLen(const Eigen::Vector3f &t1,
                                                   const Eigen::Vector3f &t2,
                                                   const Eigen::Vector3f &t3);

private:
    void proximityRepulsion(); // TODO consider separating - and
                               // below fxns

    static float dist_between_edges(const Eigen::Map<Eigen::Vector3f> &p1,
                                    const Eigen::Map<Eigen::Vector3f> &p2,
                                    const Eigen::Map<Eigen::Vector3f> &q1,
                                    const Eigen::Map<Eigen::Vector3f> &q2);

    static void solveNormalEqn(Eigen::Matrix2f A, Eigen::Vector2f b,
                               Eigen::Vector2f &out);
    static inline float distBetweenPoints(Eigen::Vector3f p1,
                                          Eigen::Vector3f p2);
    static inline void projectToLine(Eigen::Vector3f pt, Eigen::Vector3f l_p1,
                                     Eigen::Vector3f l_dir,
                                     Eigen::Vector3f &out);
    static inline void clampToLine(Eigen::Vector3f l_p1, Eigen::Vector3f l_dir,
                                   Eigen::Vector3f &pt_to_clamp);
    void collisionResolution(); // TODO consider separating

    float _timestepSize;
    std::vector<Object> &_objects;
    std::vector<std::unique_ptr<Eigen::VectorXf>>
        _nextObjectPositions; // TODO make indexing better
    std::vector<std::unique_ptr<Eigen::VectorXf>> _objectAvgVelocities;
};

#endif
