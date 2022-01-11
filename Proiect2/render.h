#pragma once
#include "loadShaders.h"
#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <GL/glew.h>
#include <gl/freeglut.h>
#include <vector>
#include <iostream>
#include "Block.h"
#include "SOIL.h"

extern GLuint VaoId, VboId, EboId, ProgramId, viewLocation, projLocation, codColLocation;
const int WIN_HEIGHT = 720;
const int WIN_WIDTH = 1280;
const float PI = 3.141592;
const float CHANGE_ANGLE_SPEED = PI / 2000;
const float MOVE_SPEED = 10;
//const float BLOCK_SIZE = 30;
const int WINDOW_POS_X = 150;
const int WINDOW_POS_Y = 300;
const int CENTER_X = WINDOW_POS_X + WIN_WIDTH / 2;
const int CENTER_Y = WINDOW_POS_Y + WIN_HEIGHT / 2;
const float RANGE = 500;
extern std::vector <Block> blocks;
extern GLuint textures[150];
void initiliaze_opengl();
void handle_keyboard(unsigned char, int, int);
void render_function();
void create_vbo();
void destroy_vbo();
void create_shaders();
void destroy_shaders();
void cleanup();
void handle_mouse(int, int, int, int);
float real_sin(float);
float real_cos(float);
GLuint LoadTexture(const char* filename);