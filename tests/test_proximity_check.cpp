#include "simulation/simulation.h"
#include <Eigen/Dense>
#include <gtest/gtest.h>
#include <iostream>

TEST(ProximityCheckTest, EdgeEdgeSimpleCase) {
    Eigen::Matrix<float, 4, 3, Eigen::RowMajor> input_data{
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {1.0, 0.7, 0.0}};

    Eigen::Map<Eigen::Vector3f> e1_v1{&input_data(0, 0)};
    Eigen::Map<Eigen::Vector3f> e1_v2{&input_data(1, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v1{&input_data(2, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v2{&input_data(3, 0)};

    float distance = 0.5;

    auto res = Simulation::dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
    ASSERT_EQ(res, distance);
}

TEST(ProximityCheckTest, EdgeEdgeClosestParallelLineCase) {
    Eigen::Matrix<float, 4, 3, Eigen::RowMajor> input_data{
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.5, 0.0}, {1.0, 0.5, 0.0}};

    Eigen::Map<Eigen::Vector3f> e1_v1{&input_data(0, 0)};
    Eigen::Map<Eigen::Vector3f> e1_v2{&input_data(1, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v1{&input_data(2, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v2{&input_data(3, 0)};

    float distance = 0.5;

    auto res = Simulation::dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
    ASSERT_EQ(res, distance);
}

TEST(ProximityCheckTest, EdgeEdgeClosestParallelLineCase2) {
    Eigen::Matrix<float, 4, 3, Eigen::RowMajor> input_data{
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.5, 0.5, 0.0}, {1.0, 0.5, 0.0}};

    Eigen::Map<Eigen::Vector3f> e1_v1{&input_data(0, 0)};
    Eigen::Map<Eigen::Vector3f> e1_v2{&input_data(1, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v1{&input_data(2, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v2{&input_data(3, 0)};

    float distance = 0.5;

    auto res = Simulation::dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
    ASSERT_EQ(res, distance);
}

TEST(ProximityCheckTest, EdgeEdgeClosestParallelLineCase3) {
    Eigen::Matrix<float, 4, 3, Eigen::RowMajor> input_data{
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.5, 0.5, 0.0}, {-0.5, 0.5, 0.0}};

    Eigen::Map<Eigen::Vector3f> e1_v1{&input_data(0, 0)};
    Eigen::Map<Eigen::Vector3f> e1_v2{&input_data(1, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v1{&input_data(2, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v2{&input_data(3, 0)};

    float distance = 0.5;

    auto res = Simulation::dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
    ASSERT_EQ(res, distance);
}

TEST(ProximityCheckTest, EdgeEdgeClosestParallelLineCase4) {
    Eigen::Matrix<float, 4, 3, Eigen::RowMajor> input_data{
        {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {-0.7, 0.5, 0.0}, {-0.5, 0.5, 0.0}};

    Eigen::Map<Eigen::Vector3f> e1_v1{&input_data(0, 0)};
    Eigen::Map<Eigen::Vector3f> e1_v2{&input_data(1, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v1{&input_data(2, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v2{&input_data(3, 0)};

    Eigen::Vector3f cp1 = e1_v1;
    Eigen::Vector3f cp2 = e2_v2;

    float distance = (cp2 - cp1).stableNorm();

    auto res = Simulation::dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
    ASSERT_EQ(res, distance);
}

TEST(ProximityCheckTest, EdgeEdgeClosestSkewLineCase) {
    Eigen::Matrix<float, 4, 3, Eigen::RowMajor> input_data{
        {0.5, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.5, 1.0}, {0.0, 0.5, -1.0}};

    Eigen::Map<Eigen::Vector3f> e1_v1{&input_data(0, 0)};
    Eigen::Map<Eigen::Vector3f> e1_v2{&input_data(1, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v1{&input_data(2, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v2{&input_data(3, 0)};

    Eigen::Vector3f cpq{0.0, 0.5, 0.0};
    float distance = (e1_v1 - cpq).stableNorm();

    auto res = Simulation::dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
    ASSERT_EQ(res, distance);
}

TEST(ProximityCheckTest, EdgeEdgeClosestSkewLineCase2) {
    Eigen::Matrix<float, 4, 3, Eigen::RowMajor> input_data{
        {-0.5, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.5, 1.0}, {0.0, 0.5, -1.0}};

    Eigen::Map<Eigen::Vector3f> e1_v1{&input_data(0, 0)};
    Eigen::Map<Eigen::Vector3f> e1_v2{&input_data(1, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v1{&input_data(2, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v2{&input_data(3, 0)};

    Eigen::Vector3f cpq{0.0, 0.5, 0.0};
    Eigen::Vector3f cpp{0.0, 0.0, 0.0};
    float distance = (cpp - cpq).stableNorm();

    auto res = Simulation::dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
    ASSERT_EQ(res, distance);
}

TEST(ProximityCheckTest, EdgeEdgeClosestSkewLineCase3) {
    Eigen::Matrix<float, 4, 3, Eigen::RowMajor> input_data{
        {-0.5, 0.0, 0.0}, {-0.2, 0.0, 0.0}, {0.0, 0.5, 1.0}, {0.0, 0.5, -1.0}};

    Eigen::Map<Eigen::Vector3f> e1_v1{&input_data(0, 0)};
    Eigen::Map<Eigen::Vector3f> e1_v2{&input_data(1, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v1{&input_data(2, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v2{&input_data(3, 0)};

    Eigen::Vector3f cpq{0.0, 0.5, 0.0};

    float distance = (e1_v2 - cpq).stableNorm();

    auto res = Simulation::dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
    ASSERT_EQ(res, distance);
}

TEST(ProximityCheckTest, EdgeEdgeClosestSimCase) {
    Eigen::Matrix<float, 4, 3, Eigen::RowMajor> input_data{
        {-0.25, -0.25, 0.8038},
        {0.25, -0.25, 0.8038},
        {5.0, -5.0, 0.0},
        {-5.0, 5.0, 0.0}};

    Eigen::Map<Eigen::Vector3f> e1_v1{&input_data(0, 0)};
    Eigen::Map<Eigen::Vector3f> e1_v2{&input_data(1, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v1{&input_data(2, 0)};
    Eigen::Map<Eigen::Vector3f> e2_v2{&input_data(3, 0)};

    float distance = 0.8038;

    auto res = Simulation::dist_between_edges(e1_v1, e1_v2, e2_v1, e2_v2);
    ASSERT_EQ(res, distance);
}