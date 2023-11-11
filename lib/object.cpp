#include "object.h"
#include <Eigen/IterativeLinearSolvers>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <set>
#include <stdexcept>

Object::Object(ObjTriMesh &objTriMesh, bool isStatic, int indexOffset,
               double particleMass)
    : _static(isStatic), _particleMass(particleMass),
      _velocities(objTriMesh.NumVertices() * 3),
      _massMatrix(objTriMesh.NumVertices() * 3, objTriMesh.NumVertices() * 3) {

    const std::vector<Vertex> &vertices = objTriMesh.VertexDataRef();
    for (auto v : vertices) {
        _positions.emplace_back(v.x, v.y, v.z);
    }

    auto faces = objTriMesh.FaceDataRef();
    std::set<std::pair<uint16_t, uint16_t>> edges;
    for (auto f : faces) {
        for (auto i = 0; i < 3; i++) {
            _face_indices.push_back(f[i] + indexOffset);
            if (f[i] < f[(i + 1) % 3]) {
                edges.emplace(f[i] + indexOffset, f[(i + 1) % 3] + indexOffset);
            } else {
                edges.emplace(f[(i + 1) % 3] + indexOffset, f[i] + indexOffset);
            }
        }
    }

    _edges =
        std::vector<std::pair<uint16_t, uint16_t>>(edges.begin(), edges.end());

    _velocities.setZero();

    _massMatrix.reserve(_positions.size() * 3);
    for (unsigned int i = 0; i < 3 * _positions.size(); i++) {
        _massMatrix.insert(i, i) = particleMass; // Non-uniform density
    }
}

void Object::calculateNextPositions(double timestep_size,
                                    Eigen::VectorXf &nextPositions) {
    Eigen::Map<Eigen::VectorXf> currentPositions(
        glm::value_ptr(_positions[0]),
        _positions.size() * 3); // TODO consider removal from function

    if (_static) {
        nextPositions = currentPositions; // TODO create static Object class
        return;
    }
    Eigen::SparseMatrix<float> A;
    Eigen::VectorXf b;
    Eigen::VectorXf dv;

    A.setZero();
    A.reserve(_positions.size() * 3);
    A = _massMatrix;

    Eigen::Vector3f gravity(0, 0, -9.81);

    b.conservativeResize(_positions.size() * 3);
    // Gravity
    for (size_t i = 0; i < _positions.size(); i++) {
        for (size_t j = 0; j < 3; j++) {
            b(i * 3 + j) = gravity[j] * _particleMass;
        }
    }

    Eigen::ConjugateGradient<Eigen::SparseMatrix<float>,
                             Eigen::Lower | Eigen::Upper,
                             Eigen::DiagonalPreconditioner<float>>
        solver;

    solver.compute(A);
    dv = solver.solve(b);

    _velocities += dv;
    nextPositions = currentPositions + (_velocities * timestep_size);
}

void Object::updatePositions(Eigen::VectorXf &nextPositions) {
    // TODO alternatives?
    assert((int)_positions.size() * 3 == (int)nextPositions.size());
    for (size_t i = 0; i < _positions.size(); i++) {
        _positions[i].x = nextPositions[i * 3];
        _positions[i].y = nextPositions[i * 3 + 1];
        _positions[i].z = nextPositions[i * 3 + 2];
    }
}

std::ostream &operator<<(std::ostream &os, const Object obj) {
    os << "Dynamic: ";
    if (obj._static) {
        os << "false\n";
    } else {
        os << "true\n";
    }
    os << "\nIndices: \n";
    for (auto i : obj._face_indices) {
        os << i << ", ";
    }
    os << std::endl;

    os << "\nVertices: \n";
    for (auto v : obj._positions) {
        os << "(" << v.x << "," << v.y << "," << v.z << ")\n";
    }
    return os;
}