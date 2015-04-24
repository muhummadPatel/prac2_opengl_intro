#ifndef STLREADER_H
#define STLREADER_H

#include <string>
#include <vector>

struct triangle{
    std::vector<float> vert1;
    std::vector<float> vert2;
    std::vector<float> vert3;
};

class stlModel{
    public:
        std::vector<triangle> triangles;
        unsigned long numTriangles;
        float * points;
        stlModel(std::string filename);
};
#endif
