#include <iostream>
#include <fstream>
#include <string>

#include "stlReader.h"

using namespace std;

void stlReader::read(string filename){
    ifstream infile(filename, ios::in|ios::binary);
    
    //Header
    char header[80];
    infile.read(header, 80);
    for(int i = 0; i < 80; i++){
        cout << header[i] << endl;
    }
    cout << "__________HEADER END_________" << endl;
    
    //read numTriangles
    unsigned long numTriangles;
    char buff[4];
    infile.read(buff, 4);
    numTriangles = *((unsigned long*)buff);
    cout << numTriangles << endl;
    cout << "___________NUMTRIANGLES END__________" << endl;
}

int main(int argc, char* argv[]){
    stlReader r;
    r.read("bunny.stl");
}
