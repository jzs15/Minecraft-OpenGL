#include <stdlib.h>
#include <math.h>
#include <iostream>
#define GLM_FORCE_RADIANS
#include "global.h"
#include "world.h"
#include "texture.h"
#include "shader_utils.h"


static World *world;

static void update_vectors() {
	forward.x = sinf(angle.x) * cosf(angle.y);
	forward.y = sinf(angle.y);
	forward.z = cosf(angle.x) * cosf(angle.y);
	forward = glm::normalize(forward);

	right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

	up = glm::cross(right, forward);
}

static int init_resources() {
	/* Create shaders */

	program = create_program("shaders/minecraft.v.glsl", "shaders/minecraft.f.glsl");

	if (program == 0)
		return 0;

	attribute_coord = get_attrib(program, "coord");
	uniform_mvp = get_uniform(program, "mvp");

	if (attribute_coord == -1 || uniform_mvp == -1)
		return 0;

	/* Create and upload the texture */
	Texture blocks("resources/textures/blocks.png");

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, blocks.getWidth(), blocks.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blocks.getData());
	glGenerateMipmap(GL_TEXTURE_2D);

	/* Create the world */

	world = new World();

	position = glm::vec3(0, CY + 1, 0);
	angle = glm::vec3(0, -0.5, 0);
	update_vectors();

	/* Create a VBO for the cursor */

	glGenBuffers(1, &cursor_vbo);

	/* OpenGL settings that do not change while running this program */

	glUseProgram(program);
	glUniform1i(uniform_texture, 0);
	glClearColor(0.6, 0.8, 1.0, 0.0);
	glEnable(GL_CULL_FACE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Use GL_NEAREST_MIPMAP_LINEAR if you want to use mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPolygonOffset(1, 1);

	glEnableVertexAttribArray(attribute_coord);

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

static void display() {
	glm::mat4 view = glm::lookAt(position, position + forward, up);
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*ww / wh, 0.01f, 1000.0f);

	glm::mat4 mvp = projection * view;

	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);

	/* Then draw chunks */

	world->render(mvp);

	/* At which voxel are we looking? */

	if (select_using_depthbuffer) {
		/* Find out coordinates of the center pixel */

		float depth;
		glReadPixels(ww / 2, wh / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		glm::vec4 viewport = glm::vec4(0, 0, ww, wh);
		glm::vec3 wincoord = glm::vec3(ww / 2, wh / 2, depth);
		glm::vec3 objcoord = glm::unProject(wincoord, view, projection, viewport);

		/* Find out which block it belongs to */

		mx = objcoord.x;
		my = objcoord.y;
		mz = objcoord.z;
		if (objcoord.x < 0)
			mx--;
		if (objcoord.y < 0)
			my--;
		if (objcoord.z < 0)
			mz--;

		/* Find out which face of the block we are looking at */

		if (fract(objcoord.x) < fract(objcoord.y))
			if (fract(objcoord.x) < fract(objcoord.z))
				face = 0; // X
			else
				face = 2; // Z
		else
			if (fract(objcoord.y) < fract(objcoord.z))
				face = 1; // Y
			else
				face = 2; // Z

		if (face == 0 && forward.x > 0)
			face += 3;
		if (face == 1 && forward.y > 0)
			face += 3;
		if (face == 2 && forward.z > 0)
			face += 3;
	}
	else {
		/* Very naive ray casting algorithm to find out which block we are looking at */

		glm::vec3 testpos = position;
		glm::vec3 prevpos = position;

		for (int i = 0; i < 100; i++) {
			/* Advance from our currect position to the direction we are looking at, in small steps */

			prevpos = testpos;
			testpos += forward * 0.1f;

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
	}

	float bx = mx;
	float by = my;
	float bz = mz;

	/* Render a box around the block we are pointing at */

	float box[24][4] = {
		{bx + 0, by + 0, bz + 0, 14},
		{bx + 1, by + 0, bz + 0, 14},
		{bx + 0, by + 1, bz + 0, 14},
		{bx + 1, by + 1, bz + 0, 14},
		{bx + 0, by + 0, bz + 1, 14},
		{bx + 1, by + 0, bz + 1, 14},
		{bx + 0, by + 1, bz + 1, 14},
		{bx + 1, by + 1, bz + 1, 14},

		{bx + 0, by + 0, bz + 0, 14},
		{bx + 0, by + 1, bz + 0, 14},
		{bx + 1, by + 0, bz + 0, 14},
		{bx + 1, by + 1, bz + 0, 14},
		{bx + 0, by + 0, bz + 1, 14},
		{bx + 0, by + 1, bz + 1, 14},
		{bx + 1, by + 0, bz + 1, 14},
		{bx + 1, by + 1, bz + 1, 14},

		{bx + 0, by + 0, bz + 0, 14},
		{bx + 0, by + 0, bz + 1, 14},
		{bx + 1, by + 0, bz + 0, 14},
		{bx + 1, by + 0, bz + 1, 14},
		{bx + 0, by + 1, bz + 0, 14},
		{bx + 0, by + 1, bz + 1, 14},
		{bx + 1, by + 1, bz + 0, 14},
		{bx + 1, by + 1, bz + 1, 14},
	};

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_CULL_FACE);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindBuffer(GL_ARRAY_BUFFER, cursor_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 24);

	/* Draw a cross in the center of the screen */

	float cross[4][4] = {
		{-0.05, 0, 0, 13},
		{+0.05, 0, 0, 13},
		{0, -0.05, 0, 13},
		{0, +0.05, 0, 13},
	};

	glDisable(GL_DEPTH_TEST);
	glm::mat4 one(1);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(one));
	glBufferData(GL_ARRAY_BUFFER, sizeof cross, cross, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 4);

	/* And we are done */

	glutSwapBuffers();
}

static void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_PAGE_UP:
		keys |= 16;
		break;
	case GLUT_KEY_PAGE_DOWN:
		keys |= 32;
		break;
	case GLUT_KEY_HOME:
		position = glm::vec3(0, CY + 1, 0);
		angle = glm::vec3(0, -0.5, 0);
		update_vectors();
		break;
	case GLUT_KEY_END:
		position = glm::vec3(0, CX * SCX, 0);
		angle = glm::vec3(0, -M_PI * 0.49, 0);
		update_vectors();
		break;
	case GLUT_KEY_F1:
		select_using_depthbuffer = !select_using_depthbuffer;
		if (select_using_depthbuffer)
			printf("Using depth buffer selection method\n");
		else
			printf("Using ray casting selection method\n");
		break;
	}
}

static void specialup(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_PAGE_UP:
		keys &= ~16;
		break;
	case GLUT_KEY_PAGE_DOWN:
		keys &= ~32;
		break;
	}
}

bool checkColllision(glm::vec3 nextPosition, glm::vec3 direction)
{
	float camSize = 0.5f;
	direction = glm::normalize(direction);
	glm::vec3 camPosition = glm::floor(nextPosition + direction * camSize);
	if (world->get(camPosition.x, camPosition.y, camPosition.z))
	{
		printf("Collision: %d %d %d\n", camPosition.x, camPosition.y, camPosition.z);
		return false;
	}
	return true;
}

static void idle() {
	static int pt = 0;
	static const float movespeed = 10;

	now = time(0);
	int t = glutGet(GLUT_ELAPSED_TIME);
	float dt = (t - pt) * 1.0e-3;
	pt = t;
	glm::vec3 nextPosition;
	glm::vec3 dirction(0.0f, 0.0f, 0.0f);

	if (keys != 0) {
		printf("%f %f %f\n", position.x, position.y, position.z);
		nextPosition = position;
		if (keys & 1)
		{
			nextPosition -= right * movespeed * dt;
			dirction -= right;
		}
		if (keys & 2)
		{
			nextPosition += right * movespeed * dt;
			dirction += right;
		}
		if (keys & 4)
		{
			nextPosition += forward * movespeed * dt;
			dirction += forward;
		}
		if (keys & 8)
		{
			nextPosition -= forward * movespeed * dt;
			dirction -= forward;
		}
		if (keys & 16)
			position.y += movespeed * dt;
		if (keys & 32)
			position.y -= movespeed * dt;
		if (checkColllision(nextPosition, dirction))
		{
			position = nextPosition;
		}
	}
	glutPostRedisplay();
}

static void motion(int x, int y) {
	static bool warp = false;
	static const float mousespeed = 0.001;

	if (!warp) {
		angle.x -= (x - ww / 2) * mousespeed;
		angle.y -= (y - wh / 2) * mousespeed;

		if (angle.x < -M_PI)
			angle.x += M_PI * 2;
		if (angle.x > M_PI)
			angle.x -= M_PI * 2;
		if (angle.y < -M_PI / 2)
			angle.y = -M_PI / 2;
		if (angle.y > M_PI / 2)
			angle.y = M_PI / 2;

		update_vectors();

		// Force the mouse pointer back to the center of the screen.
		// This causes another call to motion(), which we need to ignore.
		warp = true;
		glutWarpPointer(ww / 2, wh / 2);
	}
	else {
		warp = false;
	}
}

bool canSetBlock(int x, int y, int z)
{
	glm::vec3 camPosition = glm::floor(position);
	if (camPosition.x == x && camPosition.y == y && camPosition.z == z)
	{
		return false;
	}
	return true;
}


static void mouse(int button, int state, int x, int y) {
	if (state != GLUT_DOWN)
		return;

	// Scrollwheel
	if (button == 3 || button == 4) {
		if (button == 3)
			buildtype--;
		else
			buildtype++;

		buildtype &= 0xf;
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
	case 97:
		keys &= ~1;
		break;
	case 100:
		keys &= ~2;
		break;
	case 119:
		keys &= ~4;
		break;
	case 115:
		keys &= ~8;
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
