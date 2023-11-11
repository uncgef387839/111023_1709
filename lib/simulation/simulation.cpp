#include "simulation.h"
#include <Eigen/Dense>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <optional>
#include <stdexcept>

Simulation::Simulation(std::vector<Object> &objects, float timestep_size)
    : _timestepSize{timestep_size}, _objects(objects) {

    _nextObjectPositions.reserve(_objects.size());
    _objectAvgVelocities.reserve(_objects.size());

    for (auto obj : _objects) {
        _nextObjectPositions.emplace_back(
            new Eigen::VectorXf(obj.getNumVertices() * 3));

        _objectAvgVelocities.emplace_back(
            new Eigen::VectorXf(obj.getNumVertices() * 3));
    }
}

void Simulation::step() {

    for (size_t i = 0; i < _objects.size(); i++) {
        _objects[i].calculateNextPositions(_timestepSize,
                                           *_nextObjectPositions[i]);
    }

    proximityRepulsion();

    // collisionResolution();

    for (size_t i = 0; i < _objects.size(); i++) {
        if (!_objects[i].isStatic())
            _objects[i].updatePositions(*_nextObjectPositions[i]);
    }
}

std::ostream &operator<<(std::ostream &s, const std::pair<int, int> &p) {
    s << "(" << p.first << "," << p.second << ")";
    return s;
}

void Simulation::solveNormalEqn(Eigen::Matrix2f A, Eigen::Vector2f b,
                                Eigen::Vector2f &out) {
    out = A.fullPivHouseholderQr().solve(b);
}

float Simulation::distBetweenPoints(Eigen::Vector3f p1, Eigen::Vector3f p2) {
    return (p2 - p1).stableNorm();
}

void Simulation::projectToLine(Eigen::Vector3f pt, Eigen::Vector3f l_p1,
                               Eigen::Vector3f l_dir, Eigen::Vector3f &out) {
    out = ((pt - l_p1).dot(l_dir) / l_dir.dot(l_dir)) * l_dir + l_p1;
}

void Simulation::clampToLine(Eigen::Vector3f l_p1, Eigen::Vector3f l_dir,
                             Eigen::Vector3f &pt_to_clamp) {
    float scalar_along_line = (pt_to_clamp - l_p1)[0] / l_dir[0];
    scalar_along_line = std::clamp(scalar_along_line, 0.0f, 1.0f);
    pt_to_clamp = l_p1 + scalar_along_line * l_dir;
}

float Simulation::dist_between_edges(const Eigen::Map<Eigen::Vector3f> &p1,
                                     const Eigen::Map<Eigen::Vector3f> &p2,
                                     const Eigen::Map<Eigen::Vector3f> &q1,
                                     const Eigen::Map<Eigen::Vector3f> &q2) {

    // pre-calculating some helpful values
    const Eigen::Vector3f p21 = p2 - p1;
    const float p21_dot = p21.dot(p21);
    const Eigen::Vector3f q1p1 = q1 - p1;
    const Eigen::Vector3f q21 = q2 - q1;
    const float q21_dot = q21.dot(q21);

    Eigen::Vector3f closest_point_p, closest_point_q;

    if (p21.cross(q21).stableNorm() < 1E-7) {
        // parallel case
        // TODO Multiple possible solutions - consider when returning close pts
        // Inspired by: https://stackoverflow.com/a/67102941
        const float p_param = std::clamp(q1p1.dot(p21) / p21_dot, 0.0f, 1.0f);
        const float q_param = std::clamp(
            (p_param * p21.dot(q21) - q1p1.dot(q21)) / q21_dot, 0.0f, 1.0f);
        closest_point_p = p1 + p_param * p21;
        closest_point_q = q1 + q_param * q21;
    } else {
        // Not parallel
        // Calculate closest points on infinite lines with normal equations
        Eigen::Matrix2f A{{p21_dot, -1 * p21.dot(q21)},
                          {-1 * p21.dot(q21), q21_dot}};
        Eigen::Vector2f b{p21.dot(q1p1), -1 * q21.dot(q1p1)};
        Eigen::Vector2f out;
        solveNormalEqn(A, b, out);

        // Clamp scalars that describe point location along line to segment
        float p_param = std::clamp(out[0], 0.0f, 1.0f);
        float q_param = std::clamp(out[1], 0.0f, 1.0f);
        // Calculate point coordinates
        closest_point_p = p1 + p_param * p21;
        closest_point_q = q1 + q_param * q21;

        if (p_param != out[0] || q_param != out[1]) {
            // One or more points moved when clamped
            if (((out[0] - p_param) * p21).stableNorm() <=
                ((out[1] - q_param) * q21).stableNorm()) {
                // P moved less when clamped, project to Q to get final points
                projectToLine(closest_point_p, q1, q21, closest_point_q);
                clampToLine(q1, q21, closest_point_q);
            } else {
                // Q moved less when clamped, project to P to get final points
                projectToLine(closest_point_q, p1, p21, closest_point_p);
                clampToLine(p1, p21, closest_point_p);
            }
        }
    }

    return distBetweenPoints(closest_point_p, closest_point_q);
}

void Simulation::proximityRepulsion() {
    typedef const Eigen::Map<Eigen::Vector3f> pos_ref;

    for (size_t i = 0; i < _objects.size(); i++) {
        auto &positions_i = *_nextObjectPositions[i];
        const std::vector<uint16_t> &faces_i = _objects[i].getFaceIndices();
        const std::vector<std::pair<uint16_t, uint16_t>> &edges_i =
            _objects[i].getEdges();
        size_t num_vertices_i = _objects[i].getNumVertices();
        size_t num_faces_i = _objects[i].getNumFaces();
        const float h_ii = 2.0 * _objects[i].getThickness();

        // Self-proximity point-triangle
        for (size_t f_idx = 0; f_idx < num_faces_i; f_idx++) {
            // Grab vertex indices for current face
            uint16_t f_idx_1 = faces_i[f_idx * 3];
            uint16_t f_idx_2 = faces_i[f_idx * 3 + 1];
            uint16_t f_idx_3 = faces_i[f_idx * 3 + 2];

            // Grab positions of vertices in current face
            pos_ref t1(&positions_i[f_idx_1 * 3]);
            pos_ref t2(&positions_i[f_idx_2 * 3]);
            pos_ref t3(&positions_i[f_idx_3 * 3]);

            // Calculate face-specific intermediate values
            const Eigen::Vector3f t13 = t3 - t1;
            const Eigen::Vector3f t23 = t3 - t2;
            const Eigen::Vector3f normal = t13.cross(t23);
            // Calculate delta, where sigma is characteristic length of
            // face divided by h
            // Characteristic len of triangle is sqrt of area.
            const float delta = h_ii / sqrt(normal.stableNorm() / 2.0);

            // Calculate LHS of point-triangle projection normal equations
            const Eigen::Matrix2f A{{(t13).dot(t13), t13.dot(t23)},
                                    {t13.dot(t23), t23.dot(t23)}};

            for (uint16_t v_idx = 0; v_idx < num_vertices_i; v_idx++) {
                pos_ref t4(&positions_i[v_idx * 3]);

                if (v_idx == f_idx_1 || v_idx == f_idx_2 || v_idx == f_idx_3) {
                    continue; // point in triangle - don't compare
                }

                // Point-specific intermediate values
                const Eigen::Vector3f t43 = t3 - t4;

                // Check if point is close to the plane containing the face
                if (abs(normal.dot(t43)) < h_ii) {

                    // Calculate barycentric coordinates of point projected onto
                    // plane containing face
                    const Eigen::Vector2f b{t13.dot(t43), t23.dot(t43)};
                    Eigen::Vector2f bary_proj_point;
                    solveNormalEqn(A, b, bary_proj_point);

                    if ((bary_proj_point.array() >= (-1 * delta)).all() &&
                        (bary_proj_point.array() <= (1 + delta)).all()) {
                        // Point is proximous TODO

                        // std::cout << "Point (" << t4(0) << ", " << t4(1) <<
                        // ", "
                        //           << t4(2) << ") is proximous to triangle ("
                        //           << t1.transpose() << "), (" <<
                        //           t2.transpose()
                        //           << "), (" << t3.transpose()
                        //           << ") (self-proximity)" << std::endl;
                    }
                }
            }
        }

        // Self-proximity edge compare
        for (size_t edge_idx = 0; edge_idx < edges_i.size() - 1; edge_idx++) {
            const auto &edge1 = edges_i[edge_idx];
            pos_ref e1_v1(&positions_i[edge1.first * 3]);
            pos_ref e1_v2(&positions_i[edge1.second * 3]);

            for (size_t edge2_idx = edge_idx + 1; edge2_idx < edges_i.size();
                 edge2_idx++) {

                const auto &edge2 = edges_i[edge2_idx];
                if (edge1.first == edge2.first || edge1.first == edge2.second ||
                    edge1.second == edge2.first ||
                    edge1.second == edge2.second) {
                    continue; // shared vertex, don't compare.
                }

                pos_ref e2_v1(&positions_i[edge2.first * 3]);
                pos_ref e2_v2(&positions_i[edge2.second * 3]);
                float dist = dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);

                if (dist < h_ii) {
                    // TODO
                    // std::cout << "Edges of same object are proximate"
                    //           << std::endl;
                }
            }
        }

        for (size_t j = i + 1; j < _objects.size(); j++) {
            auto &positions_j = *_nextObjectPositions[j];
            const std::vector<uint16_t> &faces_j = _objects[j].getFaceIndices();
            const auto &edges_j = _objects[j].getEdges();
            size_t num_vertices_j = _objects[j].getNumVertices();
            size_t num_faces_j = _objects[j].getNumFaces();
            const float h_ij =
                _objects[i].getThickness() + _objects[j].getThickness();

            // Proximity point-triangle
            // object j face, object i vertices
            for (size_t f_idx_j = 0; f_idx_j < num_faces_j; f_idx_j++) {
                // Grab vertex indices for current face
                uint16_t f_idx_1 = faces_j[f_idx_j * 3];
                uint16_t f_idx_2 = faces_j[f_idx_j * 3 + 1];
                uint16_t f_idx_3 = faces_j[f_idx_j * 3 + 2];

                // Grab positions of vertices in current face
                pos_ref t1(&positions_j[f_idx_1 * 3]);
                pos_ref t2(&positions_j[f_idx_2 * 3]);
                pos_ref t3(&positions_j[f_idx_3 * 3]);

                // Calculate face-specific intermediate values
                const Eigen::Vector3f t13 = t3 - t1;
                const Eigen::Vector3f t23 = t3 - t2;
                const Eigen::Vector3f normal = t13.cross(t23);
                // Calculate delta, where sigma is characteristic length of
                // face divided by h
                // Characteristic len of triangle is sqrt of area.
                const float face_delta = h_ij / sqrt(normal.stableNorm() / 2.0);

                // Calculate LHS of point-triangle projection normal
                // equations
                const Eigen::Matrix2f A{{(t13).dot(t13), t13.dot(t23)},
                                        {t13.dot(t23), t23.dot(t23)}};

                for (size_t v_idx_i = 0; v_idx_i < num_vertices_i; v_idx_i++) {

                    pos_ref t4(&positions_i[v_idx_i * 3]);

                    // Point-specific intermediate values
                    const Eigen::Vector3f t43 = t3 - t4;

                    // Check if point is close to the plane containing face
                    if (abs(normal.dot(t43)) < h_ij) {

                        // Calculate barycentric coordinates of point
                        // projected onto plane containing face
                        const Eigen::Vector2f b{t13.dot(t43), t23.dot(t43)};
                        Eigen::Vector2f bary_proj_point_12;
                        solveNormalEqn(A, b, bary_proj_point_12);
                        float bary_proj_point_3 = 1.0f - bary_proj_point_12(0) -
                                                  bary_proj_point_12(1);

                        if ((bary_proj_point_12.array() >= (-1 * face_delta))
                                .all() &&
                            (bary_proj_point_12.array() <= (1 + face_delta))
                                .all() &&
                            bary_proj_point_3 >= (-1 * face_delta) &&
                            bary_proj_point_3 <= (1 + face_delta)) {
                            // Point is proximous TODO
                            // std::cout
                            //     << "Point (" << t4(0) << ", " << t4(1) << ",
                            //     "
                            //     << t4(2) << ") is proximous to triangle ("
                            //     << t1.transpose() << "), (" << t2.transpose()
                            //     << "), (" << t3.transpose() << ")" <<
                            //     std::endl;
                        }
                    }
                }
            }

            // object i faces, object j vertices
            for (size_t f_idx_i = 0; f_idx_i < num_faces_i; f_idx_i++) {
                // Grab vertex indices for current face
                uint16_t f_idx_1 = faces_j[f_idx_i * 3];
                uint16_t f_idx_2 = faces_j[f_idx_i * 3 + 1];
                uint16_t f_idx_3 = faces_j[f_idx_i * 3 + 2];

                // Grab positions of vertices in current face
                pos_ref t1(&positions_i[f_idx_1 * 3]);
                pos_ref t2(&positions_i[f_idx_2 * 3]);
                pos_ref t3(&positions_i[f_idx_3 * 3]);

                // Calculate face-specific intermediate values
                const Eigen::Vector3f t13 = t3 - t1;
                const Eigen::Vector3f t23 = t3 - t2;
                const Eigen::Vector3f normal = t13.cross(t23);
                // Calculate delta, where sigma is characteristic length of
                // face divided by h
                // Characteristic len of triangle is sqrt of area.
                const float face_delta = h_ij / sqrt(normal.stableNorm() / 2.0);

                // Calculate LHS of point-triangle projection normal
                // equations
                const Eigen::Matrix2f A{{(t13).dot(t13), t13.dot(t23)},
                                        {t13.dot(t23), t23.dot(t23)}};
                for (size_t v_idx_j = 0; v_idx_j < num_vertices_j; v_idx_j++) {
                    pos_ref t4(&positions_j[v_idx_j * 3]);

                    // Point-specific intermediate values
                    const Eigen::Vector3f t43 = t3 - t4;

                    // Check if point is close to the plane containing the
                    // face
                    if (abs(normal.dot(t43)) < h_ij) {

                        // Calculate barycentric coordinates of point
                        // projected onto plane containing face
                        const Eigen::Vector2f b{t13.dot(t43), t23.dot(t43)};
                        Eigen::Vector2f bary_proj_point_12;
                        solveNormalEqn(A, b, bary_proj_point_12);
                        float bary_proj_point_3 = 1.0f - bary_proj_point_12(0) -
                                                  bary_proj_point_12(1);

                        if ((bary_proj_point_12.array() >= (-1 * face_delta))
                                .all() &&
                            (bary_proj_point_12.array() <= (1 + face_delta))
                                .all() &&
                            bary_proj_point_3 >= (-1 * face_delta) &&
                            bary_proj_point_3 <= (1 + face_delta)) {
                            // Point is proximous TODO
                            // std::cout
                            //     << "Point (" << t4(0) << ", " << t4(1) << ",
                            //     "
                            //     << t4(2) << ") is proximous to triangle ("
                            //     << t1.transpose() << "), (" << t2.transpose()
                            //     << "), (" << t3.transpose() << ")" <<
                            //     std::endl;
                        }
                    }
                }
            }

            // Proximity edge compare
            for (auto edge1 : edges_i) {
                pos_ref e1_v1(&positions_i[edge1.first * 3]);
                pos_ref e1_v2(&positions_i[edge1.second * 3]);

                for (auto edge2 : edges_j) {
                    pos_ref e2_v1(&positions_j[edge2.first * 3]);
                    pos_ref e2_v2(&positions_j[edge2.second * 3]);
                    float dist = dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
                    if (dist < _objects[i].getThickness() +
                                   _objects[j].getThickness()) {
                        // TODO
                        // std::cout << "Edges are proximate" << std::endl;
                    }
                }
            }
        }
    }
}

void Simulation::collisionResolution() {
    throw std::logic_error("Not implemented yet\n");
}

void Simulation::Print() { std::cout << "Print simulation state" << std::endl; }