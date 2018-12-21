#pragma once
#include <GL/glew.h>
char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
GLuint create_program(const char* vertexfile, const char *fragmentfile, const char *geometryfile = nullptr);