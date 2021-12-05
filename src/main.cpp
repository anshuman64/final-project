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

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"

using namespace std;


//////////////////////
// Parse File
/////////////////////

void rightmultiply(const glm::mat4 & M, stack<glm::mat4> &transforms) {
    glm::mat4 &T = transforms.top();
    T = T * M;
}

// Function to read the input data values
bool readvals(stringstream &s, const int numvals, GLfloat* values) {
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void readfile(const char* filename, Scene* scene) {
    string str, cmd;
    ifstream in;

    float values[10]; // up to 10 params for positions, colors, etc.
    bool validinput; // validity of input

    // Lights
    int numused = 0;
    const int numLights = 10; // max 10 lights
    glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f);

    // Materials
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 emission = glm::vec3(0.0f, 0.0f, 0.0f);
    float shininess = 0.0f;

    in.open(filename);
    if (in.is_open()) {
        stack <glm::mat4> transforms;
        transforms.push(glm::mat4(1.0));  // identity

        getline (in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                // Ruled out comment and blank lines
                stringstream s(str);
                s >> cmd;

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
                    string filename = "./output/" + str.substr(7, str.size());
                    scene->filename = filename;
                }

                // CAMERA //
                else if (cmd == "camera") {
                    validinput = readvals(s,10,values);
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
                    if (numused == numLights) {
                        cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                    } else {
                        validinput = readvals(s, 6, values); 
                        if (validinput) {
                            glm::vec3 location = glm::vec3(values[0], values[1], values[2]);
                            glm::vec3 color = glm::vec3(values[3], values[4], values[5]);

                            Light* light = new Light(location, color, false, attenuation);
                            scene->lights.push_back(light);

                            ++numused;
                        }
                    }
                } else if (cmd == "directional") {
                    if (numused == numLights) {
                        cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
                    } else {
                        validinput = readvals(s, 6, values);
                        if (validinput) {
                            glm::vec3 location = glm::vec3(values[0], values[1], values[2]);
                            glm::vec3 color = glm::vec3(values[3], values[4], values[5]);

                            Light* light = new Light(location, color, true, attenuation);
                            scene->lights.push_back(light);

                            ++numused;
                        }
                    }
                } else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        attenuation = glm::vec3(values[0], values[1], values[2]);
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
                        Material* material = new Material(ambient, diffuse, specular, emission, shininess);
                        Triangle* triangle = new Triangle(scene->vertices[values[0]],scene->vertices[values[1]],scene->vertices[values[2]], transforms.top(), material);
                        scene->geometries.push_back(triangle);
                    }
                } else if (cmd == "sphere") {
                    validinput = readvals(s, 4, values);
                    if (validinput) {
                        Material* material = new Material(ambient, diffuse, specular, emission, shininess);
                        Sphere* sphere = new Sphere(values[0], values[1], values[2], values[3], transforms.top(), material);
                        scene->geometries.push_back(sphere);
                    }
                }

                // MATERIALS //
                else if (cmd == "diffuse") {
                  validinput = readvals(s, 3, values);
                  if (validinput) {
                      diffuse = glm::vec3(values[0], values[1], values[2]);
                  }
                } else if (cmd == "specular") {
                  validinput = readvals(s, 3, values);
                  if (validinput) {
                      specular = glm::vec3(values[0], values[1], values[2]);
                  }
                } else if (cmd == "emission") {
                  validinput = readvals(s, 3, values);
                  if (validinput) {
                      emission = glm::vec3(values[0], values[1], values[2]);
                  }
                } else if (cmd == "shininess") {
                  validinput = readvals(s, 1, values);
                  if (validinput) {
                      shininess = values[0];
                  }
                } else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                      ambient = glm::vec3(values[0], values[1], values[2]);
                    }
                }

                // TRANSFORMATIONS //
                else if (cmd == "translate") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        glm::mat4 translateMatrix = glm::translate(glm::mat4(), glm::vec3(values[0], values[1], values[2]));

                        rightmultiply(translateMatrix, transforms);
                    }
                } else if (cmd == "scale") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(values[0], values[1], values[2]));

                        rightmultiply(scaleMatrix, transforms);
                    }
                } else if (cmd == "rotate") {
                    validinput = readvals(s,4,values);
                    if (validinput) {
                        glm::vec3 axis = glm::normalize(glm::vec3(values[0], values[1], values[2]));
                        float angle = values[3] * M_PI/180.0f;
                        glm::mat4 rotateMatrix = glm::rotate(glm::mat4(), angle, axis);

                        rightmultiply(rotateMatrix, transforms);
                    }
                } else if (cmd == "pushTransform") {
                    transforms.push(transforms.top());
                } else if (cmd == "popTransform") {
                    if (transforms.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    } else {
                        transforms.pop();
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

int main(int argc, char* argv[])
{
    Scene scene;
    readfile(argv[1], &scene);

    bool  use_shadows = true;
    bool  use_mirror  = true;
    bool  use_gi = true;
    int   K = 1;
    float lambda = 0.5f;

    for (int i = 2; i < argc; i++) {
        if (string(argv[i]) == "no_shadows") {
            std::cout << "no_shadows" << std::endl;
            use_shadows = false;
        } else if (string(argv[i]) == "no_mirror") {
            std::cout << "no_mirror" << std::endl;
            use_mirror = false;
        } else if (string(argv[i]) == "no_gi") {
            std::cout << "no_gi" << std::endl;
            use_gi = false;
        } else {
            float value = atoi(argv[i]);

            if (value == 0) {
                lambda = std::stof(argv[i]);
            } else {
                K = value;
            }
        }
    }

    std::cout << "K = " << std::to_string(K) << std::endl;
    std::cout << "lambda = " << std::to_string(lambda) << std::endl;

    Image image = scene.rayTrace(use_shadows, use_mirror, use_gi, K, lambda);

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
    
    std::cout << "Saving screenshot: " << scene.filename << std::endl;
    FreeImage_Save(FIF_PNG, img, scene.filename.c_str(), 0);
    FreeImage_DeInitialise();

    return 0;
}
