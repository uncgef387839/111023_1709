#ifndef OBJECT
#define OBJECT

#include "lib/trimesh.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <glm/glm.hpp>
#include <vector>

class Object {
public:
    Object(ObjTriMesh &objtrimesh, bool isStatic, int indexOffset,
           double particleMass);
    void calculateNextPositions(double timestep_size,
                                Eigen::VectorXf &nextPositions);
    void updatePositions(Eigen::VectorXf &nextPositions);

    const bool isStatic() { return _static; }

    const size_t getIndicesMemorySize() {
        if (_face_indices.size() == 0)
            return 0;
        return _face_indices.size() * sizeof(_face_indices[0]);
    }

    uint16_t *getIndicesLocation() { return _face_indices.data(); }

    const size_t getVerticesMemorySize() {
        if (_positions.size() == 0)
            return 0;
        return _positions.size() * sizeof(_positions[0]);
    };
    glm::vec3 *getVertexLocation() { return _positions.data(); }
    const int getNumVertices() { return _positions.size(); };
    const int getNumIndices() { return _face_indices.size(); };
    const int getNumFaces() { return _face_indices.size() / 3; };
    const double getThickness() { return _thickness; }

    const std::vector<std::pair<uint16_t, uint16_t>> &getEdges() {
        return _edges;
    }

    const std::vector<uint16_t> &getFaceIndices() { return _face_indices; }

    friend std::ostream &operator<<(std::ostream &os, const Object obj);

private:
    bool _static;
    double _particleMass;
    const float _thickness = 0.1; // TODO - per instance
    std::vector<glm::vec3> _positions;
    std::vector<uint16_t> _face_indices;
    std::vector<std::pair<uint16_t, uint16_t>> _edges;

    Eigen::VectorXf _velocities;
    Eigen::SparseMatrix<float> _massMatrix;
};

#endif