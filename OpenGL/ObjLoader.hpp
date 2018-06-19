//
//  ObjLoader.hpp
//  OpenGL
//
//  Created by 余蒹葭 on 2018/5/28.
//  Copyright © 2018年 余蒹葭. All rights reserved.
//

#ifndef ObjLoader_hpp
#define ObjLoader_hpp

#include <stdio.h>
#include <iostream>
#include <GLUT/GLUT.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

class ObjLoader{
public:
    ObjLoader(string filename);//const
    void Draw();//draw
private:
    vector<vector<GLfloat>>vSets;//save vertex  coordinates
    vector<vector<GLint>>fSets;//save face index
};

#endif /* ObjLoader_hpp */
