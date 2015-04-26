#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

#include "stlModel.h"

using namespace std;

stlModel::~stlModel(){
    if(points != NULL){
        delete[] points;
    }
}

bool stlModel::read(string filename){
    ifstream infile(filename.c_str(), ios::in|ios::binary);

    if(!infile){
        cout << "DIDNT OPEN FILE" << endl;
        return false;
    }

    char buffer[80];
    infile.read(buffer, 80);


    char buffer2[4];
    infile.read(buffer2, 4);
    numTriangles = *((unsigned int*) buffer2);
    //TODO: remove this V
    cout << "number of triangles: " << numTriangles << endl;

    points = new float[numTriangles * 12];
    int c = 0;
    for(int i = 0; i < numTriangles; i++){
        char normalBuff[12];
        infile.read(normalBuff, 12);

        for(int i = 0; i < 3; i++){
            char vert1Buff[12];
            infile.read(vert1Buff, 12);
            float* v1 = (float*) vert1Buff;

            points[c] = v1[0];
            points[c+1] = v1[1];
            points[c+2] = v1[2];
            points[c+3] = 1.0f;
            c+=4;
        }

        char attrib[2];
        infile.read(attrib, 2);
    }

    return true;
}
