#include <stdlib.h>
#include <FreeImage.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

// OSX systems need their own headers
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define GLM_FORCE_RADIANS // Use of degrees is deprecated. Use radians for GLM functions
#include <glm/glm.hpp>

#include "Scene.h"


using namespace std;

//////////////////////
// Parse File
/////////////////////

// Function to read the input data values
bool readvals(stringstream &s, const int numvals, GLfloat* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void readfile(const char* filename, Scene* scene)
{
    string str, cmd;
    ifstream in;

    float values[10]; // up to 10 params for positions, colors, etc.
    bool validinput; // validity of input

    // Lights
    int numused = 0;
    const int numLights = 10; // max 10 point lights.  You can increase this if you want to add more lights.
    float lightposn[3 * numLights];
    float lightcolor[3 * numLights];
    float ambient[3];

    // Materials
    float diffuse[3];
    float specular[3];
    float emission[3];
    float shininess;

    in.open(filename);
    if (in.is_open()) {
        stack <glm::mat4> transfstack;
        transfstack.push(glm::mat4(1.0));  // identity

        getline (in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines
                stringstream s(str);
                s >> cmd;
                int i;

                // GENERAL //
                if (cmd == "size") {
                   validinput = readvals(s,2,values);
                   if (validinput) {
                       scene->width  = values[0];
                       scene->height = values[1];
                   }
               } else if (cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        scene->max_depth = values[0];
                    }
                } else if (cmd == "output") {
                    // const char* string_values;
                    // validinput = readvals(s, 1, string_values);
                    // if (validinput) {
                    //     scene->filename = string_values;
                    // }
                }

                // CAMERA //
                else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {
                        glm::vec3 eye = glm::vec3(values[0], values[1], values[2]);
                        glm::vec3 target = glm::vec3(values[3], values[4], values[5]);
                        glm::vec3 up = glm::vec3(values[6], values[7], values[8]);
                        float fovy = values[9];

                        scene->camera = new Camera(eye, target, up, fovy, scene->width, scene->height);
                    }
                }

                // LIGHTS //
                else if (cmd == "point") {
                    if (numused == numLights) { // No more Lights
                        cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                    } else {
                        validinput = readvals(s, 6, values); // Position/color for lts.
                        if (validinput) {
                            for(int i = 0; i < 3; i++){
                                lightposn[numused*3 + i] = values[i];
                                lightcolor[numused*3 + i] = values[i + 3];
                            }
                            ++numused;
                        }
                    }
                } else if (cmd == "directional") {
                    // YOUR CODE HERE.  You can use point lights as an example, or
                    // implement both your own way.
                } else if (cmd == "attenuation") {
                    // YOUR CODE HERE.

                    if (validinput) {

                    }
                } else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors
                    // YOUR CODE HERE

                    if (validinput) {

                    }
                }

                // GEOMETRY //
                else if (cmd == "maxverts") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        // Do nothing
                    }
                } else if (cmd == "vertex") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        glm::vec3 vertex = glm::vec3(values[0],values[1],values[2]);
                        scene->vertices.push_back(vertex);
                    }
                } else if (cmd == "tri") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        Triangle* triangle = new Triangle(scene->vertices[values[0]],scene->vertices[values[1]],scene->vertices[values[2]]);
                        scene->geometries.push_back(triangle);
                    }
                } else if (cmd == "sphere") {
                    validinput = readvals(s, 4, values);
                    if (validinput) {
                        Sphere* sphere = new Sphere(values[0], values[1], values[2], values[3]);
                        scene->geometries.push_back(sphere);
                    }
                }

                // MATERIALS //
                else if (cmd == "diffuse") {
                  validinput = readvals(s, 3, values);
                  if (validinput) {
                      for (i = 0; i < 3; i++) {
                          diffuse[i] = values[i];
                      }
                  }
                } else if (cmd == "specular") {
                  validinput = readvals(s, 3, values);
                  if (validinput) {
                      for (i = 0; i < 3; i++) {
                          specular[i] = values[i];
                      }
                  }
                } else if (cmd == "emission") {
                  validinput = readvals(s, 3, values);
                  if (validinput) {
                      for (i = 0; i < 3; i++) {
                          emission[i] = values[i];
                      }
                  }
                } else if (cmd == "shininess") {
                  validinput = readvals(s, 1, values);
                  if (validinput) {
                      shininess = values[0];
                  }
                }

                // TRANSFORMATIONS //
                else if (cmd == "translate") {
                    validinput = readvals(s,3,values);
                    if (validinput) {

                        glm::mat4 translateMatrix;

                        // YOUR CODE HERE
                        // Implement a translation matrix.  You can just use glm built in functions
                        // if you want.

                        // rightmultiply(translateMatrix, transfstack);
                    }
                } else if (cmd == "scale") {
                    validinput = readvals(s,3,values);
                    if (validinput) {

                        glm::mat4 scaleMatrix;

                        // YOUR CODE HERE
                        // Implement a scale matrix.  You can just use glm built in functions
                        // if you want.

                        // rightmultiply(scaleMatrix, transfstack);
                    }
                } else if (cmd == "rotate") {
                    validinput = readvals(s,4,values);
                    if (validinput) {

                        glm::vec3 axis = glm::normalize(glm::vec3(values[0], values[1], values[2]));
                        glm::mat4 rotateMatrix;

                        // YOUR CODE HERE
                        // Implement a rotation matrix.  You can just use glm built in functions
                        // if you want.

                        // rightmultiply(rotateMatrix, transfstack);

                    }
                } else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    } else {
                        transfstack.pop();
                    }
                }

                // OTHER //
                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline (in, str);
        }
    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
}

////////////////
// Main
////////////////

int main(int argc, char** argv)
{
    Scene scene;
    readfile("./testscenes/scene3.test", &scene);
    Image image = scene.rayTrace();

    // Take screenshot
    FreeImage_Initialise();
    FIBITMAP *img = FreeImage_Allocate(scene.width, scene.height, 24);
    RGBQUAD color;

    for (int j = 0; j < scene.height; j++) {
        for (int i = 0; i < scene.width; i++) {
            color.rgbRed = image[i][j].z;
            color.rgbGreen = image[i][j].y;
            color.rgbBlue = image[i][j].x;

            FreeImage_SetPixelColor(img, i, scene.height-(j+1), &color);
        }
    }
    
    // FIBITMAP* img = FreeImage_ConvertFromRawBits(image.data(), scene.width, scene.height, scene.width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);
    std::cout << "Saving screenshot: " << scene.filename << std::endl;
    FreeImage_Save(FIF_PNG, img, scene.filename, 0);
    FreeImage_DeInitialise();

    return 0;
}
