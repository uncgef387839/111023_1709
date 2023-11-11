#include "trimesh.h"
#include <fstream>  // ifstream
#include <iostream> // cout

ObjTriMesh::ObjTriMesh(std::string filename) {
    std::ifstream f(filename);

    if (!f.is_open()) {
        std::cout << "Couldn't open file (" << filename << ")\n";
        throw "BadFile";
        return;
    }

    std::string line;
    while (f.good()) {
        std::getline(f, line);
        if (line[0] == '#') {
            continue;
            // ignore comment line
        }

        std::stringstream line_ss(line);
        std::string keyword;
        line_ss >> keyword;

        if (keyword == "v") {
            Vertex v;
            line_ss >> v.x;
            line_ss >> v.y;
            line_ss >> v.z;

            vertices.push_back(v);

        } else if (keyword == "f") {
            uint v1, v2, v3;
            line_ss >> v1;
            line_ss >> v2;
            line_ss >> v3;
            Face f = {v1, v2, v3};
            faces.push_back(f);
        }
    }
}
