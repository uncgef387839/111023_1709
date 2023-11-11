#include "trimesh.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

// Only supporting the following data types:
//  - geometric vertices (v)
//  - faces (f)

TEST(ObjTriMeshTest, BadFileTest) {
    bool exception_caught = false;
    try {
        ObjTriMesh p("this_is_a_bad_file_name.obj");
    } catch (char const *s) {
        exception_caught = true;
        EXPECT_STREQ(s, "BadFile") << "The strings don't match\n";
    }
    if (!exception_caught) {
        FAIL() << "Bad file test didn't catch an exception\n";
    }
}

TEST(ObjTriMeshTest, TestDataTest) {
    std::ifstream testfile("tests/data/test.obj");

    if (!testfile) {
        FAIL() << "Couldn't open file\n";
    }
}

TEST(ObjTriMeshTest, FullReadTest) {
    ObjTriMesh poly("tests/data/test.obj");

    std::vector<Vertex> expected_vertices = {
        Vertex{-0.25, -0.25, 1.0}, Vertex{0.25, -0.25, 1.0},
        Vertex{-0.25, 0.25, 1.0}, Vertex{0.25, 0.25, 1.0}};
    std::vector<Face> expected_faces = {Face{1, 2, 3}, Face{2, 4, 3}};

    ASSERT_EQ(poly.NumVertices(), expected_vertices.size());

    const std::vector<Vertex> &vertices = poly.VertexDataRef();

    for (size_t i = 0; i < vertices.size(); i++) {
        EXPECT_EQ(vertices[i].x, expected_vertices[i].x);
        EXPECT_EQ(vertices[i].y, expected_vertices[i].y);
        EXPECT_EQ(vertices[i].z, expected_vertices[i].z);
    }

    const std::vector<Face> &faces = poly.FaceDataRef();
    std::cout << "Faces size: " << faces.size() << std::endl;

    ASSERT_EQ(poly.NumFaces(), expected_faces.size());

    for (size_t i = 0; i < faces.size(); i++) {
        for (size_t j = 0; j < 3; j++) {
            EXPECT_EQ(faces[i][j], expected_faces[i][j]);
        }
    }
}