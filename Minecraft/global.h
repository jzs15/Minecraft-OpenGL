#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#define CX 16
#define CY 32
#define CZ 16
#define SCX 32
#define SCY 2
#define SCZ 32
#define CHUNKSLOTS (SCX * SCY * SCZ)
#define SEALEVEL 4
#define M_PI 3.14159265359

#define KEY_UP 119
#define KEY_DOWN 115
#define KEY_LEFT 97
#define KEY_RIGHT 100
#define KEY_ESCAPE 27
#define KEY_SPACE 32
#define KEY_TAB 9


static GLuint program;
static GLint attribute_coord;
static GLint uniform_mvp;
static GLuint texture_id;
static GLint uniform_texture;
static GLuint cursor_vbo;

static int ww, wh;
static int mx, my, mz;
static int face;
static uint8_t buildtype = 1;

static time_t now;
static unsigned int keys;
static bool select_using_depthbuffer = false;

static const char *blocknames[16] = {
	"air", "dirt", "topsoil", "grass", "leaves", "wood", "stone", "sand",
	"water", "glass", "brick", "ore", "woodrings", "white", "black", "x-y"
};
