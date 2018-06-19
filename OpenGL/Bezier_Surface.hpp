//
//  Bezier_Surface.hpp
//  OpenGL
//
//  Created by 余蒹葭 on 2018/5/29.
//  Copyright © 2018年 余蒹葭. All rights reserved.
//

#ifndef Bezier_Surface_hpp
#define Bezier_Surface_hpp

#include <stdio.h>
#include <iostream>
#include <GLUT/GLUT.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

class Bezier_Surface{
public:
    Bezier_Surface(GLfloat ***ctrl_points);//ctrl points
    void Draw();//draw
private:
    vector<vector<GLfloat>>vSets;//save control points
    vector<vector<GLint>>fSets;//save face index
};


#endif /* Bezier_Surface_hpp */



