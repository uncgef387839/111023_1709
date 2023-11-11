#include "deligate.h"

std::ostream &operator<<(std::ostream &os, const DisplaySimulationDeligate d) {
    os << "Static Vertex Extracts: " << std::endl;
    for (auto extract : d.staticVertexExtracts) {
        os << "\tExtract size:" << extract.size_bytes << std::endl;
    }

    os << "Dynamic Vertex Extracts: " << std::endl;
    for (auto extract : d.dynamicVertexExtracts) {
        os << "\tExtract size:" << extract.size_bytes << std::endl;
    }

    os << "Static Index Extracts: " << std::endl;
    for (auto extract : d.staticIndexExtracts) {
        os << "\tExtract size:" << extract.size_bytes << std::endl;
    }

    os << "Dynamic Index Extracts: " << std::endl;
    for (auto extract : d.dynamicIndexExtracts) {
        os << "\tExtract size:" << extract.size_bytes << std::endl;
    }
    return os;
}