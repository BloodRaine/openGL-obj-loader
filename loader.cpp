
#ifdef __APPLE__ // if compiling on Mac OS
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else // else compiling on Linux OS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

// include GLM libraries and matrix functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <string> // std::string, std::stof
#include <cstdio>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <fstream> // std::ifstream
#include <sstream>

using namespace std;

struct Vertex {
    glm::vec3* position;
    glm::vec3* normal;
    glm::vec2* textureCoordinates;
};

struct Triangle {
    vector<Vertex> vertices;
};

vector<float> split(const string s, char delim) {
    stringstream ss(s);
    string item;
    vector<float> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(atof(item.c_str()));
    }
    return tokens;
}

glm::vec3* getVert(string line) {
    glm::vec3* r = new glm::vec3();
    vector<float> temp = split(line, ' ');
    r->x = temp.at(1);
    r->y = temp.at(2);
    r->z = temp.at(3);
    return r;
}

glm::vec2* getTexture(string line) {
    glm::vec2* tex = new glm::vec2();
    vector<float> temp = split(line, ' ');
    tex->x = temp.at(1);
    tex->y = temp.at(2);
    return tex;
}

Triangle* createFace(string line, vector<glm::vec3*> vertices, vector<glm::vec3*> normals, vector<glm::vec2*> textures) {
    Triangle* tri = new Triangle();
    stringstream ss(line);
    char delim = ' ';
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item.c_str());
    }
    
    // iterate over each substring
    for (string str : tokens) {
        Vertex vert;
    //     // split into each float value
        vector<float> verts = split(str, '/');
    //     // check size of float vector to determine how to parse the line
        if (verts.size() == 1) {
    //         // only the vertex is included
            vert.position = vertices.at(verts.at(0) - 1);
            tri->vertices.push_back(vert);
        } else if (verts.size() == 2) {
    //         // vertex and normal were included
            vert.position = vertices.at(verts.at(0)-1);
            vert.textureCoordinates = textures.at(verts.at(1) - 1);
            tri->vertices.push_back(vert);
        } else if (verts.size() == 3) {
            if (verts[1] == 0) {
    //             // vertex and texture coord were included
                vert.position = vertices[verts.at(0) - 1];
                vert.normal = normals[verts.at(2) - 1];
            } else {
    //             // vertex, normal, and texture coords were all included
                vert.position = vertices[verts.at(0) - 1];
                vert.textureCoordinates = textures[verts.at(1) - 1];
                vert.normal = normals[verts.at(2) - 1];
            }
            tri->vertices.push_back(vert);
        }
        
    }
    return tri;
}

vector<Triangle*> loadOBJ(string filename) {
    vector<Triangle*> faces;
    vector<glm::vec3*> vertices;
    vector<glm::vec3*> normals;
    vector<glm::vec2*> textures;
    cout << "loading file" << endl;
    ifstream file(filename.c_str());
    string line;
    int count = 0;
    while (getline(file, line)) {
        string op = "";
        if (line[0] == ' ') {
            cout << "error in file";
        } else {
            op = line.substr(0,line.find_first_of(' '));
        }

		if (op == "v") {
            vertices.push_back(getVert(line));
		} else if (op == "vn") {
            normals.push_back(getVert(line));
            // add normal
		} else if (op == "vt") {
            textures.push_back(getTexture(line));
            // add texture
		} else if (op == "f") {
            faces.push_back(createFace(line.substr(line.find_first_of(" "), line.length()), vertices, normals, textures));
            // add face
		} else if (op == "o") {
            // new object
            count++;
        }
    }
    // cout << "num verts loaded: " << vertices.size() << endl;
    // cout << "num faces loaded: " << faces.size() << endl;
    file.close();
    return faces;
}