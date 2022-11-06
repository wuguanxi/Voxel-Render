#ifndef WATER_RENDER_H
#define WATER_RENDER_H

#include "vao.h"
#include "camera.h"
#include "shader.h"

class WaterRender {
private:
	VAO vao;
	GLsizei vertex_count;
	vec3 position = vec3(0, 0, 0);
	quat rotation = quat(1, 0, 0, 0);
public:
	WaterRender(vector<vec2> vertices);
	void setWorldTransform(vec3 position, quat rotation);
	void draw(Shader& shader, Camera& camera);
};

#endif
