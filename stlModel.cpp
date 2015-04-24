#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "stlModel.h"

using namespace std;

vector<triangle> triangles;
unsigned long numTriangles;

stlModel::stlModel(string filename){
    ifstream infile(filename, ios::in|ios::binary);
    
    char* headerBuff = new char[80];
    infile.read(headerBuff, 80);
    delete[] headerBuff;
    
    char* numTrianglesBuff = new char[4];
    infile.read(numTrianglesBuff, 4);
    numTriangles = *((unsigned long*) numTrianglesBuff);
    delete[] numTrianglesBuff;
    
    cout << "numTris " << numTriangles << endl;
    
    for(int i = 0; i < numTriangles; i++){
         char* normalBuff = new char[12];
         infile.read(normalBuff, 12);
         
         char* vert1Buff = new char[12];
         infile.read(vert1Buff, 12);
         float* v1 = (float*) vert1Buff;
         
         char* vert2Buff = new char[12];
         infile.read(vert2Buff, 12);
         float* v2 = (float*) vert2Buff;
         
         char* vert3Buff = new char[12];
         infile.read(vert3Buff, 12);
         float* v3 = (float*) vert3Buff;
         
         triangle t;
         t.vert1.push_back(v1[0]);
         t.vert1.push_back(v1[1]);
         t.vert1.push_back(v1[2]);
         
         t.vert2.push_back(v2[0]);
         t.vert2.push_back(v2[1]);
         t.vert2.push_back(v2[2]);
         
         t.vert3.push_back(v3[0]);
         t.vert3.push_back(v3[1]);
         t.vert3.push_back(v3[2]);
         
         triangles.push_back(t);
         
         char* attribBuff = new char[2];
         infile.read(attribBuff, 2);
         
         delete[] normalBuff;
         delete[] vert1Buff;
         delete[] vert2Buff;
         delete[] vert3Buff;
         delete[] attribBuff;
    }
    
    for(auto tri = triangles.begin(); tri < triangles.begin() + 10; ++tri){
        cout << "v1: " << (*tri).vert1[0] << " " << (*tri).vert1[1] << " " << (*tri).vert1[2] << " " << endl;
        cout << "v2: " << (*tri).vert2[0] << " " << (*tri).vert2[1] << " " << (*tri).vert2[2] << " " << endl;
        cout << "v3: " << (*tri).vert3[0] << " " << (*tri).vert3[1] << " " << (*tri).vert3[2] << " " << endl;
        cout << "-------------------------------" << endl;
    }
    
    infile.close();
}

int main(int argc, char* argv[]){
    stlModel r("bunny.stl");
}
