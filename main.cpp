#include "src/skybox.h"
#include "src/xml_loader.h"

using namespace std;
using namespace glm;

const float FAR_PLANE = 500;
const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 720;

// TODO: transparent glass

GLFWwindow* InitOpenGL(const char* window_title) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	//glfwWindowHint(GLFW_SAMPLES, 8); // MSAA
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, window_title, NULL, NULL);
	if (window == NULL) {
		printf("[GLFW] Failed to initialize OpenGL\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	return window;
}

Camera camera;
bool fullscreen = false;

void fullscreen_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void)window; (void)scancode; (void)mods;
    if (action == GLFW_RELEASE) return;
    if (key == GLFW_KEY_F11) {
		fullscreen = !fullscreen;
		if (fullscreen) {
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			glViewport(0, 0, mode->width, mode->height);
			camera.updateScreenSize(mode->width, mode->height);
		} else {
			glfwSetWindowMonitor(window, NULL, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
			glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
			camera.updateScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		}
	}
}

int main() {
	GLFWwindow* window = InitOpenGL("OpenGL");
	Shader voxel_shader("shaders/mesh_vert.glsl", "shaders/mesh_frag.glsl");
	//Shader voxel_shader("shaders/voxel_vert.glsl", "shaders/voxel_frag.glsl");
	Shader rope_shader("shaders/rope_vert.glsl", "shaders/rope_frag.glsl");
	Shader water_shader("shaders/water_vert.glsl", "shaders/water_frag.glsl");
	Shader voxbox_shader("shaders/voxbox_vert.glsl", "shaders/voxbox_frag.glsl");
	Shader skybox_shader("shaders/skybox_vert.glsl", "shaders/skybox_frag.glsl");

	Skybox skybox(skybox_shader, (float)WINDOW_WIDTH / WINDOW_HEIGHT);
	camera.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, vec3(0, 2.5, 10));
	Scene scene("main.xml");

	// FPS counter
	double dt = 0;
	double prev_time = 0;
	double actual_time = 0;
	unsigned int counter = 0;

	// Flags
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetKeyCallback(window, fullscreen_callback);

	while (!glfwWindowShouldClose(window)) {
		actual_time = glfwGetTime();
		dt = actual_time - prev_time;
		counter++;
		if (dt >= 1.0) {
			int fps = (1.0 / dt) * counter;
			float ms = dt / counter * 1000;
			char window_title[128];
			sprintf(window_title, "OpenGL %s | FPS: %d | %.1f ms", glGetString(GL_VERSION), fps, ms);
			glfwSetWindowTitle(window, window_title);
			prev_time = actual_time;
			counter = 0;
		}
		glClearColor(0.35, 0.54, 0.8, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		camera.handleInputs(window);
		camera.updateMatrix(45, 0.1, FAR_PLANE);

		scene.draw(voxel_shader, camera);
		scene.drawVoxbox(voxbox_shader, camera);
		scene.drawRope(rope_shader, camera);
		glEnable(GL_BLEND);
		scene.drawWater(water_shader, camera);
		glDisable(GL_BLEND);

		skybox.Draw(skybox_shader, camera);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
