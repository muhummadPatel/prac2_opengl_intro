/*
 * Implementation of stlModel class.
 *
 * Muhummad Yunus Patel     27-April-2015
 */

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

#include "stlModel.h"

using namespace std;

//destructor to clean up the points array
stlModel::~stlModel(){
    if(points != NULL){
        delete[] points;
    }
}

//function to read in binary stl file with the given filename
bool stlModel::read(string filename){
    ifstream infile(filename.c_str(), ios::in|ios::binary);

    if(!infile){
        cout << "DIDNT OPEN FILE" << endl;
        return false;
    }

    //read in the header
    char buffer[80];
    infile.read(buffer, 80);

    //read in the number of triangles
    char buffer2[4];
    infile.read(buffer2, 4);
    numTriangles = *((unsigned int*) buffer2);

    cout << "number of triangles: " << numTriangles << endl;

    //read in the triangles
    points = new float[numTriangles * 12];
    int c = 0;
    for(int i = 0; i < numTriangles; i++){
        //read and discard the normal vector
        char normalBuff[12];
        infile.read(normalBuff, 12);

        //read each vertex of the triangle and add coords to points array
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

        //read and discard the 2 byte attribute
        char attrib[2];
        infile.read(attrib, 2);
    }

    infile.close();
    return true;
}
