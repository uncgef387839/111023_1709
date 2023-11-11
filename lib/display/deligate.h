#ifndef DELIGATE
#define DELIGATE

#include "lib/object.h"
#include <glm/glm.hpp>
#include <vector>

struct Extract {
    Extract(void *hostMemory, size_t size)
        : hostMemory(hostMemory), size_bytes(size){};
    void *hostMemory;
    size_t size_bytes;
};

struct DisplaySimulationDeligate {

    std::vector<Extract> dynamicVertexExtracts;
    std::vector<Extract> staticVertexExtracts;

    std::vector<Extract> dynamicIndexExtracts;
    std::vector<Extract> staticIndexExtracts;

    size_t total_static_vertex_size_bytes;
    size_t total_dynamic_vertex_size_bytes;
    size_t total_static_index_size_bytes;
    size_t total_dynamic_index_size_bytes;

    size_t _numStaticIndices;
    size_t _numDynamicIndices;

    DisplaySimulationDeligate(std::vector<Object> &objects) {
        for (size_t i = 0; i < objects.size(); i++) {
            Object &object = objects[i];
            uint16_t idx_mem_size = object.getIndicesMemorySize();
            uint16_t vtx_mem_size = object.getVerticesMemorySize();

            if (!object.isStatic()) {

                dynamicIndexExtracts.emplace_back(object.getIndicesLocation(),
                                                  idx_mem_size);
                total_dynamic_index_size_bytes += idx_mem_size;
                dynamicVertexExtracts.emplace_back(object.getVertexLocation(),
                                                   vtx_mem_size);
                total_dynamic_vertex_size_bytes += vtx_mem_size;
                _numDynamicIndices += object.getNumIndices();

            } else {
                staticIndexExtracts.emplace_back(object.getIndicesLocation(),
                                                 idx_mem_size);
                total_static_index_size_bytes += idx_mem_size;

                staticVertexExtracts.emplace_back(object.getVertexLocation(),
                                                  vtx_mem_size);
                total_static_vertex_size_bytes += vtx_mem_size;
                _numStaticIndices += object.getNumIndices();
            }
        }
    }
};

std::ostream &operator<<(std::ostream &os, DisplaySimulationDeligate d);

#endif