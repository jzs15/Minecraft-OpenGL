#pragma once
#pragma comment(lib, "irrklang.lib")

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include <irrKlang/irrKlang.h>

using namespace irrklang;

#define CX 16
#define CY 32
#define CZ 16
#define SCX 32
#define SCY 4
#define SCZ 32
#define CHUNKSLOTS (SCX * SCY * SCZ)
#define SEALEVEL 4
#define ONE_DAY 600000
#define M_PI 3.14159265359

#define KEY_UP 119
#define KEY_DOWN 115
#define KEY_LEFT 97
#define KEY_RIGHT 100
#define KEY_ESCAPE 27
#define KEY_SPACE 32
#define KEY_ENTER 13
#define KEY_TAB 9
#define KEY_SHIFT 112
#define KEY_F 102
#define KEY_Z 122
#define KEY_INPUT 96
#define KEY_BACKSPACE 8

extern ISoundEngine *SoundEngine;

extern GLuint cur_program;
static GLuint program;
static GLuint hud;
static GLuint skybox;
static GLuint block_texture_id;
static GLuint sky_texture_id;
static GLuint text_texture_id;
static GLint uniform_texture;
static GLuint cursor_vbo;
static GLuint skybox_vao;
static GLuint skybox_vbo;

static int ww, wh;
static int mx, my, mz;
static int face;
static uint8_t buildtype = 1;

static float cur_time;
static int fps = 0;
static time_t now;
static unsigned int keys;
static bool select_using_depthbuffer = false;
static bool enter_press = false;
static bool is_ortho = false;
static bool is_input = false;
extern bool is_zoom;

static char input_text[1024];
static const char *blocknames[44] = {
	"Air", "Stone", "Grass Block", "Dirt", "Cobblestone", "Planks", "Sand", "Glass", 
	"Bricks", "Water", "Coal Ore", "Iron Ore", "Gold Ore", "Diamond Ore", "Emetald Ore", "Redstone Ore",
	"Lapis Ore", "Coal Block", "Iron Block", "Gold Block", "Diamond Block", "Emetald Block", "Redstone Block", "Lapis Block",
	"Oak Leaves", "Oak Log", "Jungle Leaves", "Jungle Log", "TNT", "White Block", "Yellow Block","Green Block",
	"Blue Block", "Red Block", "Black Block", "Dandelion", "Daisy", "Orange Tulip", "Pink Tulip", "Poppy", 
	"Brown Mushroom", "Red Mushroom", "Grass", "Torch"
};
