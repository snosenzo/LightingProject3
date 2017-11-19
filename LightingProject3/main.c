//
//  main.c
//  LightingProject3
//
//  Created by Sam Nosenzo on 11/16/17.
//  Copyright © 2017 Sam Nosenzo. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "initShader.h"
#include "viewFuncs.h"
#include "shapeVecs.h"
#include "objFuncs.h"


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
mat4 identity;
vec4 colors[2000000];
vec4 total_vertices[2000000];
obj objs[300];
material mats[30];
int numObjs = 0;
int screenHeight = 512;
int screenWidth = 512;
int bufferCounter = 0;
int num_vertices;
GLuint translate_loc;
GLuint scale_loc;
GLuint rotation_loc;
GLuint mvm_loc;
GLuint proj_mat_loc;
float thetaX, thetaY, thetaZ;
mat4 mvm;

mat4 proj_mat;
float p_near = -.1;
float p_far = -100; //-2.2;
float p_left = .3;
float p_right = .1;
float p_top = .1;
float p_bot = .3;
vec4 light_loc;

vec4 _eye, _at, _up;
float eyeTheta = 0;
float camHeight = 0;

//lighting variables
vec4 light_diffuse, light_specular, light_ambient;



void rectObj(float xScale, float yScale, float zScale, float x, float y, float z, obj *in) {
    
    in->vertices = (vec4 *)malloc(sizeof(vec4) * 24);
    in->num_verts = 24;
    for(int j = 0; j < in->num_verts; j++) {
        in->vertices[j].x = cubeVertices[j].x * xScale;
        in->vertices[j].y = cubeVertices[j].y * yScale;
        in->vertices[j].z = cubeVertices[j].z * zScale;
        in->vertices[j].w = cubeVertices[j].w;
    }
    num_vertices += 24;
    getTranslationMatrix(x, y, z, &in->translation);
    in->rotation = identity;
    in->scale = 1;
    in->mesh_type = GL_QUADS;
    numObjs++;
}

void addObjectColor(obj *o, int numPointsPerColor, float r, float g, float b) {
    o->colors = (vec4 *)malloc(sizeof(vec4) * o->num_verts);
    
    for(int i = 0; i < o->num_verts/numPointsPerColor; i++) {
        for(int j = 0; j < numPointsPerColor; j++) {
            o->colors[i*numPointsPerColor + j].x = r;
            o->colors[i*numPointsPerColor + j].y = g;
            o->colors[i*numPointsPerColor + j].z = b;
            o->colors[i*numPointsPerColor + j].w = 1.0;
        }
    }
}

void initObjs() {
    //    sphereObj(1, 0, 0, 0, &(objs[0]), &(objs[1]), &(objs[2]));
    
    tetrahedron(5, &(objs[0]));
    numObjs++;
    num_vertices+= objs[0].num_verts;
    getTranslationMatrix(0, .5, 0, &(objs[0].translation));
    objs[0].rotation = identity;
    objs[0].scale = .5;
    addObjectColor(&objs[0], 3, 1, 0, 0);
    defineVector(1, 0, 0, 1, &(mats[0].reflect_ambient));
    defineVector(1, 0, 0, 1, &(mats[0].reflect_diffuse));
    defineVector(1, 1, 1, 1, &(mats[0].reflect_specular));
    mats[0].shininess = 10;
    
    tetrahedron(5, &(objs[1]));
    numObjs++;
    num_vertices+= objs[1].num_verts;
    getTranslationMatrix(1, .5, 0, &(objs[1].translation));
    objs[1].rotation = identity;
    objs[1].scale = .5;
    addObjectColor(&objs[1], 3, .2, 1, 0);
    defineVector(0, 1, 0, 1, &(mats[1].reflect_ambient));
    defineVector(0, 1, 0, 1, &(mats[1].reflect_diffuse));
    defineVector(1, 1, 1, 1, &(mats[1].reflect_specular));
    mats[1].shininess = 10;
    
    tetrahedron(5, &(objs[2]));
    numObjs++;
    num_vertices+= objs[2].num_verts;
    getTranslationMatrix(2, .5, 0, &(objs[2].translation));
    objs[2].rotation = identity;
    objs[2].scale = .5;
    addObjectColor(&objs[2], 3, 0, 0, .7);
    defineVector(0, 0, 1, 1, &(mats[2].reflect_ambient));
    defineVector(0, 0, 1, 1, &(mats[2].reflect_diffuse));
    defineVector(1, 1, 1, 1, &(mats[2].reflect_specular));
    mats[2].shininess = 10;
    
    tetrahedron(5, &(objs[3]));
    numObjs++;
    num_vertices+= objs[3].num_verts;
    getTranslationMatrix(3, .5, 0, &(objs[3].translation));
    objs[3].rotation = identity;
    objs[3].scale = .5;
    addObjectColor(&objs[3], 3, 1, 1, 0);
    defineVector(0, 1, 0, 1, &(mats[3].reflect_ambient));
    defineVector(0, 1, 0, 1, &(mats[3].reflect_diffuse));
    defineVector(1, 1, 1, 1, &(mats[3].reflect_specular));
    mats[3].shininess = 10;
    
    tetrahedron(5, &(objs[4]));
    numObjs++;
    num_vertices+= objs[4].num_verts;
    getTranslationMatrix(4, .5, 0, &(objs[4].translation));
    objs[4].rotation = identity;
    objs[4].scale = .5;
    addObjectColor(&objs[4], 3, 1, .7, 0);
    defineVector(1, .5, 0, 1, &(mats[4].reflect_ambient));
    defineVector(1, .5, 0, 1, &(mats[4].reflect_diffuse));
    defineVector(1, 1, 1, 1, &(mats[4].reflect_specular));
    mats[4].shininess = 10;
    
    tetrahedron(3, &(objs[5]));
    numObjs++;
    num_vertices+= objs[5].num_verts;
    getTranslationMatrix(light_loc.x, light_loc.y, light_loc.z, &(objs[5].translation));
    objs[5].rotation = identity;
    objs[5].scale = .1;
    addObjectColor(&objs[5], 3, 1, 1, 1);
    
    rectObj(20, .01, 20, 0, -.03, 0, &objs[6]);
    addObjectColor(&objs[6], 4, 0, .5, 0);
   
    
}




//void initColors(int num, int numPoints, int b_doShadows) {
//    if(b_doShadows){
//        colors = (vec4 *) malloc(sizeof(vec4) * num * 2);
//    } else {
//        colors = (vec4 *) malloc(sizeof(vec4) * num);
//    }
//    for(int i = 0; i < num/numPoints; i++) {
//        float x, y, z;
//        x = (float)rand()/(float)(RAND_MAX);
//        y = (float)rand()/(float)(RAND_MAX);
//        z = (float)rand()/(float)(RAND_MAX);
//        for(int j = 0; j < numPoints; j++) {
//            colors[i*numPoints + j].x = x;
//            colors[i*numPoints + j].y = y;
//            colors[i*numPoints + j].z = z;
//            colors[i*numPoints + j].w = 1.0;
//        }
//    }
//
//    for(int i = num; i < (num * 2); i++) {
//
//        colors[i].x = 0;
//        colors[i].y = 0;
//        colors[i].z = 0;
//        colors[i].w = 1.0;
//    }
//
//}

vec4 getShadowVec(const vec4 p, const vec4 light, float yPlane) {
    vec4 res;
    res.x = light.x - (light.y - yPlane) * ((light.x - p.x) / (light.y - p.y));
    res.y = yPlane;
    res.z = light.z - (light.y - yPlane) * ((light.z - p.z) / (light.y - p.y));
    res.w = 1.0;
    return res;
}

vec4 getTransformedVec(const vec4 v, float scale, mat4 rot, mat4 trans) {
    vec4 temp, res;
    res.x = v.x * scale;
    res.y = v.y * scale;
    res.z = v.z * scale;
    res.w = v.w;
    
    multMatrixVector(&rot, &res, &temp);
    multMatrixVector(&trans, &temp, &res);
    return res;
}

void loadObjectOnBuffer(obj *o, int offset){
    //    glBufferSubData(GL_ARRAY_BUFFER, offset, o.num_verts*sizeof(vec4), o.vertices);
    for(int i = 0; i < o->num_verts; i++ ) {
        colors[i+offset] = o->colors[i];
        total_vertices[i+offset] = getTransformedVec(o->vertices[i], o->scale, o->rotation, o->translation);
    }
    bufferCounter+=sizeof(vec4) * o->num_verts;
    o->buffer_start_loc = offset;
}

void loadShadowsOnBuffer(obj *o, int offset) {
    vec4 dark;
    defineVector(0, 0, 0, 1, &dark);
    for(int i = 0; i < o->num_verts; i++ ) {
        colors[i+num_vertices+o->buffer_start_loc] = dark;
        total_vertices[i+num_vertices+o->buffer_start_loc] = getShadowVec(total_vertices[i + o->buffer_start_loc], light_loc, 0);
    }
    bufferCounter+=sizeof(vec4) * o->num_verts;
    o->shadow_loc = offset + num_vertices;
}

void init(void)
{
    
    defineVector(1, 1, 1, 1, &light_diffuse);
    defineVector(1, 1, 1, 1, &light_specular);
    defineVector(0.2, .2, .2, 1, &light_ambient);
    defineVector(0, 3, 0, 1, &light_loc);
    defineVector(0, 5, 4, 0, &_eye);
    defineVector(0, .25, 0, 1, &_at);
    defineVector(0, 1, 0, 1, &_up);
    look_at(_eye.x, _eye.y, _eye.z, _at.x, _at.y, _at.z, _up.x, _up.y, _up.z, &mvm);
    perspective(p_right, p_top, p_near, p_far, &proj_mat);
    

    initObjs();
//    initColors(num_vertices, 1, 1);
    
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    GLuint vao;
    glGenVertexArraysAPPLE(1, &vao);
    glBindVertexArrayAPPLE(vao);
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*num_vertices*4, NULL, GL_STATIC_DRAW);
    
    int offset = 0;
    for(int i = 0; i < numObjs; i++ ){
        loadObjectOnBuffer(&(objs[i]), offset);
        offset+=objs[i].num_verts;
    }
    
    offset = 0;
    for(int i = 0; i < (numObjs-2); i++ ){
        loadShadowsOnBuffer(&(objs[i]), offset);
        offset+=objs[i].num_verts;
    }
    
    int bufferSpace = sizeof(vec4) * num_vertices * 2;
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4) * num_vertices * 2, total_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, bufferSpace, sizeof(vec4) * num_vertices * 2, colors);
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) (sizeof(vec4) * (num_vertices * 2)));
    
    translate_loc = glGetUniformLocation(program, "translate");
    scale_loc = glGetUniformLocation(program, "scale");
    rotation_loc = glGetUniformLocation(program, "rotation");
    mvm_loc = glGetUniformLocation(program, "mvm");
    proj_mat_loc = glGetUniformLocation(program, "proj_mat");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void) {
    mat4 temp0, temp1;
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    
    glUniformMatrix4fv(mvm_loc, 1, GL_FALSE, &mvm);
    glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, &proj_mat);
    scalarMultMatrix(&identity, .1, &temp0);
    glUniformMatrix4fv(scale_loc, 1, GL_FALSE, &identity);
    glUniformMatrix4fv(rotation_loc, 1, GL_FALSE, &identity);
    glUniformMatrix4fv(translate_loc, 1, GL_FALSE, &identity);
    for(int i = 0; i < numObjs; i++) {
        glDrawArrays(objs[i].mesh_type, objs[i].buffer_start_loc, objs[i].num_verts);
    }
    for(int i = 0; i < (numObjs-2); i++) {
        glDrawArrays(objs[i].mesh_type, objs[i].shadow_loc, objs[i].num_verts);
    }
    glutSwapBuffers();
    
}

void keyboard(unsigned char key, int mousex, int mousey) {
    // MVM keys
    if(key =='d') {
        _eye.x+=.05;
        look_at(_eye.x, _eye.y, _eye.z, _at.x, _at.y, _at.z, _up.x, _up.y, _up.z, &mvm);
    }
    if(key =='a') {
        _eye.x-=.05;
        look_at(_eye.x, _eye.y, _eye.z, _at.x, _at.y, _at.z, _up.x, _up.y, _up.z, &mvm);
    }
    if(key =='r') {
        _eye.y+=.05;
        look_at(_eye.x, _eye.y, _eye.z, _at.x, _at.y, _at.z, _up.x, _up.y, _up.z, &mvm);
    }
    if(key =='f') {
        _eye.y-=.05;
        look_at(_eye.x, _eye.y, _eye.z, _at.x, _at.y, _at.z, _up.x, _up.y, _up.z, &mvm);
    }
    if(key =='w') {
        _eye.z+=.05;
        look_at(_eye.x, _eye.y, _eye.z, _at.x, _at.y, _at.z, _up.x, _up.y, _up.z, &mvm);
    }
    if(key =='s') {
        _eye.z-=.05;
        //        look_at_theta(eyeTheta, camHeight, &mvm);
        look_at(_eye.x, _eye.y, _eye.z, _at.x, _at.y, _at.z, _up.x, _up.y, _up.z, &mvm);
    }
    
    if(key == 'q')
    exit(0);
    glutPostRedisplay();
}

void mouseFunction(int button, int state, int x, int y) {
    
}

void dragFunction(int x, int y) {
    
}

void idle() {
    
    glutPostRedisplay();
}

int main(int argc, const char * argv[]) {
    
    
    getIdentityMatrix(&identity);
    multiplyMatrices(&identity, &identity, &mvm);
    multiplyMatrices(&identity, &identity, &proj_mat);
    srand(time(NULL));
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lighting Project");
    
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseFunction);
    glutMotionFunc(dragFunction);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}


