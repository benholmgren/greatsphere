#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <csci441/shader.h>
#include <csci441/matrix4.h>
#include <csci441/matrix3.h>
#include <csci441/vector4.h>
#include <csci441/uniform.h>

#include "shape.h"
#include "model.h"
#include "camera.h"
#include "renderer.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
int normalls=0;

float crossx(const float x1,const float y1,const float z1,const float x2,const float y2,const float z2,const float x3,const float y3,const float z3){
    
    float v1x=x2-x1;
    float v1y=y2-y1;
    float v1z=z2-z1;
    float v2x=x3-x1;
    float v2y=y3-y1;
    float v2z=z3-z1;
    
    float i = (v1y*v2z-v1z*v2y);
    float j = -1*(v1x*v2z-v1z*v2x);
    float k = (v1x*v2y-v1y*v2x);
    float norm = sqrt(i*i+j*j+k*k);
    return i/norm;
    
}

float crossy(const float x1,const float y1,const float z1,const float x2,const float y2,const float z2,const float x3,const float y3,const float z3){
    
    float v1x=x2-x1;
    float v1y=y2-y1;
    float v1z=z2-z1;
    float v2x=x3-x1;
    float v2y=y3-y1;
    float v2z=z3-z1;
    
    float i = (v1y*v2z-v1z*v2y);
    float j = -1*(v1x*v2z-v1z*v2x);
    float k = (v1x*v2y-v1y*v2x);
    float norm = sqrt(i*i+j*j+k*k);
    return j/norm;
    
}


float crossz(const float x1,const float y1,const float z1,const float x2,const float y2,const float z2,const float x3,const float y3,const float z3){
    
    float v1x=x2-x1;
    float v1y=y2-y1;
    float v1z=z2-z1;
    float v2x=x3-x1;
    float v2y=y3-y1;
    float v2z=z3-z1;
    
    float i = (v1y*v2z-v1z*v2y);
    float j = -1*(v1x*v2z-v1z*v2x);
    float k = (v1x*v2y-v1y*v2x);
    float norm = sqrt(i*i+j*j+k*k);
    return k/norm;
    
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool isPressed(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool isReleased(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_RELEASE;
}

Matrix4 processModel(const Matrix4& model, GLFWwindow *window) {
    Matrix4 trans;

    const float ROT = 1;
    const float SCALE = .05;
    const float TRANS = .01;


    // ROTATE
    if (isPressed(window, GLFW_KEY_U)) { trans.rotate_x(-ROT); }
    else if (isPressed(window, GLFW_KEY_I)) { trans.rotate_x(ROT); }
    else if (isPressed(window, GLFW_KEY_O)) { trans.rotate_y(-ROT); }
    else if (isPressed(window, GLFW_KEY_P)) { trans.rotate_y(ROT); }
    else if (isPressed(window, '[')) { trans.rotate_z(-ROT); }
    else if (isPressed(window, ']')) { trans.rotate_z(ROT); }
    // SCALE
    else if (isPressed(window, '-')) { trans.scale(1-SCALE, 1-SCALE, 1-SCALE); }
    else if (isPressed(window, '=')) { trans.scale(1+SCALE, 1+SCALE, 1+SCALE); }
    // TRANSLATE
    else if (isPressed(window, GLFW_KEY_UP)) { trans.translate(0, TRANS, 0); }
    else if (isPressed(window, GLFW_KEY_DOWN)) { trans.translate(0, -TRANS, 0); }
    else if (isPressed(window, GLFW_KEY_LEFT)) { trans.translate(-TRANS, 0, 0); }
    else if (isPressed(window, GLFW_KEY_RIGHT)) { trans.translate(TRANS, 0, 0); }
    else if (isPressed(window, ',')) { trans.translate(0,0,TRANS); }
    else if (isPressed(window, '.')) { trans.translate(0,0,-TRANS); }
 

    return trans * model;
}

void processInput(Matrix4& model, GLFWwindow *window) {
    if (isPressed(window, GLFW_KEY_ESCAPE) || isPressed(window, GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    }
    
    model = processModel(model, window);
    
}
/*
void updateNormals(GLFWwindow *window){
    if (!isReleased(window, GLFW_KEY_SPACE)&&normalls==0) {
     
        normalls=normalls+1;
    }else if(!isReleased(window, GLFW_KEY_SPACE)) {
        
        normalls=normalls+1;}
}
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        normalls=normalls+1;
    }
}
void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int main(void) {
    GLFWwindow* window;

    glfwSetErrorCallback(errorCallback);

    /* Initialize the library */
    if (!glfwInit()) { return -1; }

    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CSCI441-lab", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // tell glfw what to do on resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // init glad
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    // create obj
    std::vector<float> flat;
    //change the first argument to a bigger number for a better image and
    //a smaller number for it to render way faster and look significantly worse
    std::vector<float> myvector = Sphere(200, 1, 1, .2, .4).coords;
    //std::vector<float> myvector = TorSphere(30, 1, 1, .2, .4).coords;
    //Torus c(20, .75, .25, 1, .2, .4);
    
    
    
    int count = 0;
    float nx;
    float ny;
    float nz;
    for (std::vector<float>::iterator it = myvector.begin() ; it != myvector.end(); ++it){
        if(count % 27 ==0){
            
            float x1=*it;
            std::advance(it,1);
            float y1=*it;
            std::advance(it,1);
            float z1=*it;
            std::advance(it,7);
            float x2=*it;
            std::advance(it,1);
            float y2=*it;
            std::advance(it,1);
            float z2=*it;
            std::advance(it,7);
            float x3=*it;
            std::advance(it,1);
            float y3=*it;
            std::advance(it,1);
            
            float z3=*it;
            std::advance(it,-20);
            nx=crossx(x1,y1,z1,x2,y2,z2,x3,y3,z3);
            ny=crossy(x1,y1,z1,x2,y2,z2,x3,y3,z3);
            nz=crossz(x1,y1,z1,x2,y2,z2,x3,y3,z3);
        }
        if(count % 9==6){
            flat.push_back(nx);
        }else if(count % 9 == 7){
            flat.push_back(ny);
        }else if(count % 9 == 8){
            flat.push_back(nz);
        }else{
            flat.push_back(*it);
        }
        
        count = count +1;
    }
    //Now do the shaded
    float mx;
    float my;
    float mz;
    count = 0;
    std::vector<float> shaded;
    for (std::vector<float>::iterator it1 = flat.begin() ; it1 != flat.end(); ++it1){
        if(count % 9==0){
            float x=*it1;
            std::advance(it1,1);
            float y=*it1;
            std::advance(it1,1);
            float z=*it1;
            std::advance(it1,-2);
            
            
            std::vector<float> xnormals;
            std::vector<float> ynormals;
            std::vector<float> znormals;
            
            for (std::vector<float>::iterator it2 = flat.begin() ; it2 != flat.end(); it2=it2+9){
                
                
                float x1=*it2;
                std::advance(it2,1);
                float y1=*it2;
                std::advance(it2,1);
                float z1=*it2;
                std::advance(it2,-2);
                
                if(x==x1 && y==y1 && z==z1){
                    std::advance(it2,6);
                    xnormals.push_back(*it2);
                    std::advance(it2,1);
                    ynormals.push_back(*it2);
                    std::advance(it2,1);
                    znormals.push_back(*it2);
                    std::advance(it2,-8);
                }
            
            }
            
            //average the normals
            float sum=0;
            float nxx;
            for (std::vector<float>::iterator it3 = xnormals.begin() ; it3 != xnormals.end(); ++it3){
                sum=sum+*it3;
            }
            nxx=sum/xnormals.size();
            sum=0;
            float nyy;
            for (std::vector<float>::iterator it4 = ynormals.begin() ; it4 != ynormals.end(); ++it4){
                sum=sum+*it4;
            }
            nyy=sum/ynormals.size();
            sum=0;
            float nzz;
            for (std::vector<float>::iterator it5 = znormals.begin() ; it5 != znormals.end(); ++it5){
                sum=sum+*it5;
            }
            nzz=sum/znormals.size();
            
            float length= sqrt(nxx*nxx+nyy*nyy+nzz*nzz);
            mx=nxx/length;
            my=nyy/length;
            mz=nzz/length;
        }
        if(count % 9==6){
            shaded.push_back(mx);
        }else if(count % 9 == 7){
            shaded.push_back(my);
        }else if(count % 9 == 8){
            shaded.push_back(mz);
        }else{
            shaded.push_back(*it1);
        }
        count = count + 1;
    }
    

    std::vector<float> workit;
    //if(normalls % 2 == 0){workit=flat;}
    //else{workit=shaded;}

    Model obj1(
              shaded,
            Shader("../vert.glsl", "../frag.glsl"));
    Model obj2(
               flat,
               Shader("../vert.glsl", "../frag.glsl"));

    // make a floor
    /*
    Model floor(
            DiscoCube().coords,
            Shader("../vert.glsl", "../frag.glsl"));
    Matrix4 floor_trans, floor_scale;
    floor_trans.translate(0, -2, 0);
    floor_scale.scale(100, 1, 100);
    floor.model = floor_trans*floor_scale;
     */

    // setup camera
    Matrix4 projection;
    projection.perspective(45, 1, .01, 10);

    Camera camera;
    camera.projection = projection;
    camera.eye = Vector4(0, 0, 3);
    camera.origin = Vector4(0, 0, 0);
    camera.up = Vector4(0, 1, 0);

    // and use z-buffering
    glEnable(GL_DEPTH_TEST);

    // create a renderer
    Renderer renderer;

    // set the light position
    Vector4 lightPos(3.75f, 3.75f, 4.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
       
        //updateNormals(window);
        
        glfwSetKeyCallback(window, key_callback);
       
        // process input
        if(normalls%2==0){
            
            processInput(obj1.model, window);
        }else{processInput(obj2.model,window);}
        
        
       
    
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the object and the floor
        if(normalls%2==0){
            
            renderer.render(camera, obj1, lightPos);
        }else{renderer.render(camera, obj2, lightPos);}
        
        //renderer.render(camera, floor, lightPos);

        /* Swap front and back and poll for io events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
