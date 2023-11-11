#ifndef TRIMESH
#define TRIMESH

#include <array>
#include <sstream>
#include <string>
#include <vector>

struct Vertex {
    float x;
    float y;
    float z;
};

typedef std::array<uint, 3> Face;

class ObjTriMesh {
public:
    ObjTriMesh(std::string filename);
    size_t NumVertices() { return vertices.size(); };
    size_t NumFaces() { return faces.size(); };
    const Vertex *VertexData() { return vertices.data(); };
    const Face *FaceData() { return faces.data(); };
    const std::vector<Vertex> &VertexDataRef() { return vertices; };
    const std::vector<Face> &FaceDataRef() { return faces; };

private:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};
#endif