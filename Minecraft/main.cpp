#include <stdlib.h>
#include <math.h>
#include <iostream>
#define GLM_FORCE_RADIANS
#include "global.h"
#include "world.h"
#include "texture.h"
#include "shader_utils.h"
#include "camera.h"

static World *world;
static Camera *camera;
bool is_zoom = false;
GLuint cur_program;

//ISoundEngine *SoundEngine = createIrrKlangDevice();
//SoundEngine->play2D("audio/breakout.mp3", GL_TRUE);

static void init_skybox()
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f, -1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,

		-1.0f, -1.0f,  1.0f, 2.0f,
		-1.0f, -1.0f, -1.0f, 2.0f,
		-1.0f,  1.0f, -1.0f, 2.0f,
		-1.0f,  1.0f, -1.0f, 2.0f,
		-1.0f,  1.0f,  1.0f, 2.0f,
		-1.0f, -1.0f,  1.0f, 2.0f,

		 1.0f, -1.0f, -1.0f, 3.0f,
		 1.0f, -1.0f,  1.0f, 3.0f,
		 1.0f,  1.0f,  1.0f, 3.0f,
		 1.0f,  1.0f,  1.0f, 3.0f,
		 1.0f,  1.0f, -1.0f, 3.0f,
		 1.0f, -1.0f, -1.0f, 3.0f,

		-1.0f, -1.0f,  1.0f, 4.0f,
		-1.0f,  1.0f,  1.0f, 4.0f,
		 1.0f,  1.0f,  1.0f, 4.0f,
		 1.0f,  1.0f,  1.0f, 4.0f,
		 1.0f, -1.0f,  1.0f, 4.0f,
		-1.0f, -1.0f,  1.0f, 4.0f,

		-1.0f,  1.0f, -1.0f, 5.0f,
		 1.0f,  1.0f, -1.0f, 5.0f,
		 0.0f,  1.0f,  0.0f, 5.0f,

		 0.0f,  1.0f,  0.0f, 6.0f,
		 1.0f,  1.0f, -1.0f, 6.0f,
		 1.0f,  1.0f,  1.0f, 6.0f,

		 1.0f,  1.0f,  1.0f, 7.0f,
		 0.0f,  1.0f,  0.0f, 7.0f,
		-1.0f,  1.0f,  1.0f, 7.0f,

		-1.0f,  1.0f, -1.0f, 8.0f,
		 0.0f,  1.0f,  0.0f, 8.0f,
		-1.0f,  1.0f,  1.0f, 8.0f,

		-1.0f,  -1.0f, -1.0f, 5.0f,
		 1.0f,  -1.0f, -1.0f, 5.0f,
		 0.0f,  -1.0f,  0.0f, 5.0f,

		 0.0f, -1.0f,  0.0f, 6.0f,
		 1.0f, -1.0f, -1.0f, 6.0f,
		 1.0f, -1.0f,  1.0f, 6.0f,

		 1.0f, -1.0f,  1.0f, 7.0f,
		 0.0f, -1.0f,  0.0f, 7.0f,
		-1.0f, -1.0f,  1.0f, 7.0f,

		-1.0f, -1.0f, -1.0f, 8.0f,
		 0.0f, -1.0f,  0.0f, 8.0f,
		-1.0f, -1.0f,  1.0f, 8.0f,
	}; 
	glGenVertexArrays(1, &skybox_vao);
	glGenBuffers(1, &skybox_vbo);
	glBindVertexArray(skybox_vao);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));

	
	glGenTextures(1, &sky_texture_id);
	glBindTexture(GL_TEXTURE_2D, sky_texture_id);
	Texture skyTexture("resources/textures/sky.png");
	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, skyTexture.getWidth(), skyTexture.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, skyTexture.getData());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	

	glUseProgram(skybox);
	glUniform1i(glGetUniformLocation(skybox, "skybox"), 0);
}

static void init_text()
{
	Texture text_texture("resources/textures/text.png");
	glGenTextures(1, &text_texture_id);
	glBindTexture(GL_TEXTURE_2D, text_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_texture.getWidth(), text_texture.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, text_texture.getData());
	glGenerateMipmap(GL_TEXTURE_2D);
}

static int init_resources() {
	/* Create shaders */

	program = create_program("shaders/minecraft.v.glsl", "shaders/minecraft.f.glsl");
	hud = create_program("shaders/hud.v.glsl", "shaders/hud.f.glsl");
	skybox = create_program("shaders/skybox.v.glsl", "shaders/skybox.f.glsl");
	text = create_program("shaders/hud.v.glsl", "shaders/hud.f.glsl");

	if (program == 0 || hud == 0 || skybox == 0 || text == 0)
		return 0;

	cur_time = -0.3;
	init_skybox();
	init_text();
	/* Create and upload the texture */
	Texture blocks("resources/textures/blocks.png");

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &block_texture_id);
	glBindTexture(GL_TEXTURE_2D, block_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, blocks.getWidth(), blocks.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blocks.getData());
	glGenerateMipmap(GL_TEXTURE_2D);

	/* Create the world */

	world = new World();
	camera = new Camera(glm::vec3(0, CY + 1, 0), glm::vec3(0.0f, -1.0f, 0.0f));

	/* Create a VBO for the cursor */

	glGenBuffers(1, &cursor_vbo);

	/* OpenGL settings that do not change while running this program */

	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "blockTexture"), 0);
	glClearColor(0.6, 0.8, 1.0, 0.0);
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_CULL_FACE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Use GL_NEAREST_MIPMAP_LINEAR if you want to use mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPolygonOffset(1, 1);

	glEnableVertexAttribArray(glGetAttribLocation(program, "coord"));

	return 1;
}

static void reshape(int w, int h) {
	ww = w;
	wh = h;
	glViewport(0, 0, w, h);
}

// Not really GLSL fract(), but the absolute distance to the nearest integer value
static float fract(float value) {
	float f = value - floorf(value);
	if (f > 0.5)
		return 1 - f;
	else
		return f;
}

static void drawHud() {
	glDisable(GL_DEPTH_TEST);
	glUseProgram(hud);
	/* Draw a cross in the center of the screen */
	float cross_height = 20.0f / wh;
	float cross_weight = 20.0f / ww;
	float cross[4][4] = {
		{-cross_weight, 0, 0, 2},
		{cross_weight, 0, 0, 2},
		{0, -cross_height, 0, 2},
		{0, cross_height, 0, 2},
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof cross, cross, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(hud, "coord"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 4);
	float hudWidth = 150.0f / ww;
	float widthGap = 1.0 - 50.0f / ww;
	float hudHeight = 150.0f / wh;
	float heightGap = 1.0 - 50.0f / wh;
	float u = (buildtype % 8) / 8.0f;
	float v = int(buildtype / 8) / 8.0f;
	float textureGap = 1 / 8.0f;
	float blocksVertex[4][4] = {
		{-widthGap, -heightGap, u, v + textureGap},	
		{-widthGap, hudHeight - heightGap, u, v},
		{hudWidth - widthGap, hudHeight - heightGap, u + textureGap, v},
		{hudWidth - widthGap, -heightGap, u + textureGap, v + textureGap},
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof blocksVertex, blocksVertex, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(hud, "coord"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_QUADS, 0, 4);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program);
}

static void calculateFrameRate()
{
	static float FramePerSecond = 0.0f;
	static float lastTime = 0.0f;
	float curTime = GetTickCount() * 0.001f;
	++FramePerSecond;
	if (curTime - lastTime > 1.0f)
	{
		lastTime = curTime;
		fps = FramePerSecond;
		FramePerSecond = 0;
	}
}

static void drawText()
{
	char txt[1024];
	calculateFrameRate();
	glm::vec3 pos = camera->getPosition();
	int time = 720 * cur_time + 720;
	int hour = time / 60;
	int min = time % 60;

	snprintf(txt, 1024, "(%.2f, %.2f, %.2f) %.2d:%.2d %dfps", pos.x, pos.y, pos.z, hour, min, fps);

	glBindTexture(GL_TEXTURE_2D, text_texture_id);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(text);

	float textWidth = 20.0f / ww;
	float textHeight = 20.0f / wh;
	float widthGap = 1 - 5.0f / ww;
	float heightGap = 1.0 - 5.0f / wh;
	float textureGap = 1 / 16.0f;

	int len = strlen(txt);
	for (int i = 0; i < len; i++)
	{
		float u = (txt[i] % 16) / 16.0f;
		float v = int(txt[i] / 16) / 16.0f;
		float blocksVertex[4][4] = {
			{-widthGap, heightGap, u, v},												//left top
			{-widthGap, heightGap - textHeight, u, v + textureGap},					//left bottom
			{textWidth - widthGap, heightGap - textHeight, u + textureGap, v + textureGap},			//right bottom
			{textWidth - widthGap, heightGap, u + textureGap, v},		//right top
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof blocksVertex, blocksVertex, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(text, "coord"), 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_QUADS, 0, 4);
		widthGap -= 17.0f / ww;
	}
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, block_texture_id);
}

static void drawInputText()
{
	glBindTexture(GL_TEXTURE_2D, text_texture_id);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(text);

	float textWidth = 25.0f / ww;
	float textHeight = 25.0f / wh;
	float widthGap = 1 - 5.0f / ww;
	float heightGap = 1.0 - 35.0f / wh;
	float textureGap = 1 / 16.0f;

	int len = strlen(input_text);
	for (int i = 0; i < len; i++)
	{
		float u = (input_text[i] % 16) / 16.0f;
		float v = int(input_text[i] / 16) / 16.0f;
		float blocksVertex[4][4] = {
			{-widthGap, heightGap, u, v},												//left top
			{-widthGap, heightGap - textHeight, u, v + textureGap},					//left bottom
			{textWidth - widthGap, heightGap - textHeight, u + textureGap, v + textureGap},			//right bottom
			{textWidth - widthGap, heightGap, u + textureGap, v},		//right top
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof blocksVertex, blocksVertex, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(text, "coord"), 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_QUADS, 0, 4);
		widthGap -= 20.0f / ww;
	}
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, block_texture_id);
}

static void display() {
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection;
	if (is_ortho && !is_zoom)
	{
		projection = glm::ortho(0.0f, 200.0f, -100.0f, 200.0f, -1000.0f, 1000.0f);
	}
	else if (!is_ortho && is_zoom)
	{
		projection = glm::perspective(glm::radians(10.0f), 1.0f*ww / wh, 0.01f, 1000.0f);
	}
	else
	{
		projection = glm::perspective(glm::radians(45.0f), 1.0f*ww / wh, 0.01f, 1000.0f);
	}
	glm::mat4 sky_mvp = projection * glm::mat4(glm::mat3(view));
	glm::mat4 mvp = projection * view;

	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);

	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(skybox);
	glUniform1f(glGetUniformLocation(skybox, "timeValue"), cur_time);
	glUniformMatrix4fv(glGetUniformLocation(skybox, "mvp"), 1, GL_FALSE, glm::value_ptr(sky_mvp));
	// skybox cube
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glBindVertexArray(skybox_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky_texture_id);
	glDrawArrays(GL_TRIANGLES, 0, 48);
	glBindVertexArray(0);
	glUseProgram(program);
	glDepthFunc(GL_LESS);


	glUniform1f(glGetUniformLocation(program, "timeValue"), cur_time);
	/* Then draw chunks */
	cur_program = program;
	world->render(mvp);

	/* At which voxel are we looking? */


	glm::vec3 testpos = camera->getPosition();
	glm::vec3 prevpos = testpos;
	glm::vec3 front = camera->getFront();

	for (int i = 0; i < 100; i++) {
		/* Advance from our currect position to the direction we are looking at, in small steps */

		prevpos = testpos;
		testpos += front * 0.1f;

		mx = floorf(testpos.x);
		my = floorf(testpos.y);
		mz = floorf(testpos.z);

		/* If we find a block that is not air, we are done */

		if (world->get(mx, my, mz))
			break;
	}

	/* Find out which face of the block we are looking at */

	int px = floorf(prevpos.x);
	int py = floorf(prevpos.y);
	int pz = floorf(prevpos.z);

	if (px > mx)
		face = 0;
	else if (px < mx)
		face = 3;
	else if (py > my)
		face = 1;
	else if (py < my)
		face = 4;
	else if (pz > mz)
		face = 2;
	else if (pz < mz)
		face = 5;

	/* If we are looking at air, move the cursor out of sight */
	if (!world->get(mx, my, mz))
	{
		mx = my = mz = 99999;
	}

	float bx = mx;
	float by = my;
	float bz = mz;

	/* Render a box around the block we are pointing at */
	float box[24][5] = {
		{bx + 0, by + 0, bz + 0, 63, 0},
		{bx + 1, by + 0, bz + 0, 63, 0},
		{bx + 0, by + 1, bz + 0, 63, 0},
		{bx + 1, by + 1, bz + 0, 63, 0},
		{bx + 0, by + 0, bz + 1, 63, 0},
		{bx + 1, by + 0, bz + 1, 63, 0},
		{bx + 0, by + 1, bz + 1, 63, 0},
		{bx + 1, by + 1, bz + 1, 63, 0},

		{bx + 0, by + 0, bz + 0, 63, 0},
		{bx + 0, by + 1, bz + 0, 63, 0},
		{bx + 1, by + 0, bz + 0, 63, 0},
		{bx + 1, by + 1, bz + 0, 63, 0},
		{bx + 0, by + 0, bz + 1, 63, 0},
		{bx + 0, by + 1, bz + 1, 63, 0},
		{bx + 1, by + 0, bz + 1, 63, 0},
		{bx + 1, by + 1, bz + 1, 63, 0},

		{bx + 0, by + 0, bz + 0, 63, 0},
		{bx + 0, by + 0, bz + 1, 63, 0},
		{bx + 1, by + 0, bz + 0, 63, 0},
		{bx + 1, by + 0, bz + 1, 63, 0},
		{bx + 0, by + 1, bz + 0, 63, 0},
		{bx + 0, by + 1, bz + 1, 63, 0},
		{bx + 1, by + 1, bz + 0, 63, 0},
		{bx + 1, by + 1, bz + 1, 63, 0},
	};

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_CULL_FACE);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	glBindBuffer(GL_ARRAY_BUFFER, cursor_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "coord"), 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_BYTE, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(glGetAttribLocation(program, "coord"));
	glDrawArrays(GL_LINES, 0, 24);


	drawHud();
	drawText();
	if (is_input)
	{
		drawInputText();
	}
	/* And we are done */

	glutSwapBuffers();
}

static void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		keys |= 64;
		break;
	case GLUT_KEY_RIGHT:
		keys |= 128;
		break;
	case GLUT_KEY_UP:
		keys |= 256;
		break;
	case GLUT_KEY_DOWN:
		keys |= 512;
		break;
	case KEY_SHIFT:
		camera->changeType();
		break;
	}
}

static void specialup(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		keys &= ~64;
		break;
	case GLUT_KEY_RIGHT:
		keys &= ~128;
		break;
	case GLUT_KEY_UP:
		keys &= ~256;
		break;
	case GLUT_KEY_DOWN:
		keys &= ~512;
		break;
	}
}

static void idle() {
	static int pt = 0;
	now = time(0);
	int t = glutGet(GLUT_ELAPSED_TIME);
	float gap = t - pt;
	float dt = gap * 1.0e-3;
	pt = t;
	cur_time += 2.0 * gap / ONE_DAY;
	cur_time = cur_time >= 1.0 ? glm::fract(cur_time) - 1 : cur_time;

	if (keys != 0)
	{
		camera->processKeyboard(keys, dt, world);
	}
	camera->gravity(dt, world);
	glutPostRedisplay();
}

static void motion(int x, int y) {
	camera->processMouseMovement(x - ww / 2, y - wh / 2);
	glutWarpPointer(ww / 2, wh / 2);
}

bool canSetBlock(int x, int y, int z)
{
	if (!world->canSetBlock(x, y, z, buildtype))
		return false;
	glm::vec3 pos = camera->getPosition();
	if (pos.x + 0.25 <= x)
		return true;
	if (x + 1 <= pos.x - 0.25)
		return true;
	if (pos.y + 0.1 <= y)
		return true;
	if (y + 1 <= pos.y - 1.4)
		return true;
	if (pos.z + 0.25 <= z)
		return true;
	if (z + 1 <= pos.z - 0.25)
		return true;
	return false;
}

static void mouse(int button, int state, int x, int y) {
	if (state != GLUT_DOWN)
		return;

	// Scrollwheel
	if (button == 3 || button == 4) {
		if (button == 3)
		{
			if (buildtype == 0)
			{
				buildtype = 43;
			}
			else
			{
				buildtype--;
			}
		}
		else
		{
			if (buildtype == 43)
			{
				buildtype = 0;
			}
			else
			{
				buildtype++;
			}
		}
		fprintf(stderr, "Building blocks of type %u (%s)\n", buildtype, blocknames[buildtype]);
		return;
	}

	fprintf(stderr, "Clicked on %d, %d, %d, face %d, button %d\n", mx, my, mz, face, button);

	if (button == 0) {
		if (face == 0)
			mx++;
		if (face == 3)
			mx--;
		if (face == 1)
			my++;
		if (face == 4)
			my--;
		if (face == 2)
			mz++;
		if (face == 5)
			mz--;
		if (canSetBlock(mx, my, mz))
		{
			world->set(mx, my, mz, buildtype);
		}
	}
	else {
		world->set(mx, my, mz, 0);
	}
}

void processNormalKeys(unsigned char key, int x, int y)
{
	if (is_input)
	{
		if (key == KEY_ESCAPE)
		{
			memset(input_text, 0, sizeof(input_text));
			is_input = !is_input;
			return;
		}
		else if (key == KEY_ENTER)
		{
			is_input = !is_input;
			//load rendering function
			memset(input_text, 0, sizeof(input_text));
			return;
		}
		snprintf(input_text, 1024, "%s%c", input_text, key);
		return;
	}
	switch (key) {
	case KEY_LEFT:
		keys |= 1;
		break;
	case KEY_RIGHT:
		keys |= 2;
		break;
	case KEY_UP:
		keys |= 4;
		break;
	case KEY_DOWN:
		keys |= 8;
		break;
	case KEY_SPACE:
		keys |= 16;
		break;
	case KEY_Z:
		//keys |= 32;
		if (!is_zoom)
		{
			is_zoom = !is_zoom;
		}
		break;
	case KEY_F:
		if (!is_ortho)
		{
			is_ortho = !is_ortho;
		}
		break;
	case KEY_INPUT:
		if (!is_input)
		{
			is_input = !is_input;
		}
		break;
	case KEY_ENTER:
		if (!enter_press)
		{
			world->set(mx, my, mz, 0);
			enter_press = !enter_press;
		}
		break;
	case KEY_ESCAPE:
		exit(0);
		break;
	default:
		break;
	}
}

void processNormalUpKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case KEY_LEFT:
		keys &= ~1;
		break;
	case KEY_RIGHT:
		keys &= ~2;
		break;
	case KEY_UP:
		keys &= ~4;
		break;
	case KEY_DOWN:
		keys &= ~8;
		break;
	case KEY_SPACE:
		keys &= ~16;
		break;
	case KEY_Z:
		//keys &= ~32;
		if (is_zoom)
		{
			is_zoom = !is_zoom;
		}
		break;
	case KEY_F:
		if (is_ortho)
		{
			is_ortho = !is_ortho;
		}
		break;
	case KEY_ENTER:
		if (enter_press)
		{
			enter_press = !enter_press;
		}
		break;
	default:
		break;
	}
}

static void free_resources() {
	glDeleteProgram(program);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Minecraft");

	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}

	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "No support for OpenGL 2.0 found\n");
		return 1;
	}

	printf("Use the mouse to look around.\n");
	printf("Use cursor keys, pageup and pagedown to move around.\n");
	printf("Use home and end to go to two predetermined positions.\n");
	printf("Press the left mouse button to build a block.\n");
	printf("Press the right mouse button to remove a block.\n");
	printf("Use the scrollwheel to select different types of blocks.\n");
	printf("Press F1 to toggle between depth buffer and ray casting methods for cube selection.\n");
	if (init_resources()) {
		glutSetCursor(GLUT_CURSOR_NONE);
		glutWarpPointer(320, 240);
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutIdleFunc(display);
		glutKeyboardFunc(processNormalKeys);
		glutKeyboardUpFunc(processNormalUpKeys);
		glutSpecialFunc(special);
		glutSpecialUpFunc(specialup);
		glutIdleFunc(idle);
		glutPassiveMotionFunc(motion);
		glutMotionFunc(motion);
		glutMouseFunc(mouse);
		glutMainLoop();
	}

	free_resources();
	return 0;
}