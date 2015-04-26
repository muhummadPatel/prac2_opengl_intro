#ifndef STLMODEL_H
#define STLMODEL_H

#include <string>

class stlModel{
    public:
        unsigned long numTriangles;
        float* points;
        stlModel(): numTriangles(0), points(NULL){}
        ~stlModel();
        bool read(std::string filename);
};

#endif // STLMODEL_H
