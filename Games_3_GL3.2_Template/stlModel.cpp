#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "stlModel.h"

using namespace std;

stlModel::stlModel(string filename){
    numTriangles = 0;
    ifstream infile(filename, ios::in|ios::binary);
    
    char* headerBuff = new char[80];
    infile.read(headerBuff, 80);
    delete[] headerBuff;
    
    char* numTrianglesBuff = new char[4];
    infile.read(numTrianglesBuff, 4);
    numTriangles = *((unsigned long*) numTrianglesBuff);
    delete[] numTrianglesBuff;
    
    cout << "numTris " << numTriangles << endl;
    points = new float[120 * 9];
    int c = 0;
    for(int i = 0; i < 120; i++){
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
         
         points[c] = v1[0];
         points[c+1] = v1[1];
         points[c+2] = v1[2];
         points[c+3] = v2[0];
         points[c+4] = v2[1];
         points[c+5] = v2[2];
         points[c+6] = v3[0];
         points[c+7] = v3[1];
         points[c+8] = v3[2];
         c+=9;
         
         char* attribBuff = new char[2];
         infile.read(attribBuff, 2);
         
         delete[] normalBuff;
         delete[] vert1Buff;
         delete[] vert2Buff;
         delete[] vert3Buff;
         delete[] attribBuff;
    }
    
//    c=0;
//    for(auto tri = triangles.begin(); tri < triangles.begin() + 10; ++tri){
//        cout << "v1: " << (*tri).vert1[0] << " " << (*tri).vert1[1] << " " << (*tri).vert1[2] << " " << endl;
//        cout << "pointsv1: " << points[c] << " " << points[c+1] << " " << points[c+2] << " " << endl;
//        cout << "v2: " << (*tri).vert2[0] << " " << (*tri).vert2[1] << " " << (*tri).vert2[2] << " " << endl;
//        cout << "v3: " << (*tri).vert3[0] << " " << (*tri).vert3[1] << " " << (*tri).vert3[2] << " " << endl;
//        c+=9;
//        cout << "-------------------------------" << endl;
//    }
    
    infile.close();
}

//int main(int argc, char* argv[]){
//stlModel bunny("bunny.stl");
//return 0;
//}
