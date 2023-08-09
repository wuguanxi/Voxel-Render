#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#include "vao.h"
#include "camera.h"

using namespace std;

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

class UI_Rectangle {
private:
	VAO vao;
public:
	UI_Rectangle();
	void draw(Shader& shader, GLuint texture_id, float offset_x, float offset_y);
};

GLFWwindow* InitOpenGL(const char* window_title);
void key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

string GetScenePath(int argc, char* argv[]);

void PushTexture(GLuint texture_id, Shader& shader, const char* uniform, GLuint unit = 0);

#endif
