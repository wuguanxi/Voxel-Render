#ifndef WATER_RENDER_H
#define WATER_RENDER_H

#include "vao.h"
#include "camera.h"
#include "shader.h"
#include <glm/gtc/quaternion.hpp>

using namespace glm;

struct AABB_2D {
	vec2 min;
	vec2 max;
};

class WaterRender {
private:
	VAO vao;
	GLsizei vertex_count;
	vec3 position = vec3(0, 0, 0);
	AABB_2D bounding_box;
public:
	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthTexture;

	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	GLuint dudv_texture;
	GLuint normal_texture;

	WaterRender(vector<vec2> vertices);
	float GetHeight();
	void BindReflectionFB();
	void BindrefractionFB();
	void UnbindFB(Camera& camera);
	void setWorldTransform(vec3 position);
	void draw(Shader& shader, Camera& camera);
};

#endif
