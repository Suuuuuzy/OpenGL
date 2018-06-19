//
//  ObjLoader.cpp
//  OpenGL
//
//  Created by 余蒹葭 on 2018/5/28.
//  Copyright © 2018年 余蒹葭. All rights reserved.
//

#include "ObjLoader.hpp"

ObjLoader::ObjLoader(string filename){
    ifstream fin(filename, ios::in);
    if(!fin.is_open()) cout << "open failed!" << endl;
    string line;
    string number[3];
    GLint i,j, end = 0;
    GLfloat x,y,z; // store the three coordinates
    GLint in1, in2, in3; // store the three index
    
    vSets.insert(vSets.end(),{0.0,0.0,0.0}); // set the index start from 1
    fSets.insert(fSets.end(),{0,0,0}); // set the index start from 1
    
    while(!fin.eof()){ // for each line in the file
        getline(fin,line);
        cout << line << endl;
        j = 0;
        end = 0;
        number[0] = "";
        number[1] = "";
        number[2] = "";
        for(i=2; i<line.length(); i++){ // jump the first char
            if(j>=3) break;
            if(line[i]==' '){
                j++;
                end = 0;
            }
            if(line[i]=='/'){
                end=1;
            }
            else if(end==0 && line[i]!=' ')
                number[j] += line[i];
        }
        if(line[0]=='v' && line[1]==' '){ // vertexes, add to vSets
            x = atof(number[0].c_str());
            y = atof(number[1].c_str());
            z = atof(number[2].c_str());
            vSets.insert(vSets.end(),{x,y,z});//在向量末尾追加新元素，下标从1开始
        }
        else if(line[0]=='f'&& line[1]==' '){ //faces, add to fSets
            in1 = atoi(number[0].c_str());
            in2 = atoi(number[1].c_str());
            in3 = atoi(number[2].c_str());
            fSets.insert(fSets.end(),{in1,in2,in3});//在向量末尾追加新元素，下标从1开始
        }
        else continue;
    }
}

void ObjLoader::Draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat x[3], y[3], z[3];
    GLint i,j;
    glBegin(GL_LINE_LOOP);
    for(i = 1; i<fSets.size(); i++){
        for(j=0; j<3; j++){
            x[j] = vSets[fSets[i][j]][0];
            y[j] = vSets[fSets[i][j]][1];
            z[j] = vSets[fSets[i][j]][2];
            cout << x[j] << y[j] << z[j]<< endl;
        }
        glColor3f(1.0, 1.0, 1.0);
        glVertex3f(x[0],y[0],z[0]);
        glVertex3f(x[1],y[1],z[1]);
        glVertex3f(x[2],y[2],z[2]);
    }
    glEnd();
}//draw


//    vector<vector<GLfloat>>vSets;
//    vector<vector<GLint>>fSets;





