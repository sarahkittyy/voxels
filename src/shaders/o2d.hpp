#pragma once

namespace shaders {

constexpr const char* o2d_vs() {
	return R"/shader(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 norm;

out vec3 posV;
out vec2 uvV;
out vec3 normV;

uniform mat4 model;
uniform mat4 proj;

void main() {
	gl_Position = proj * model * vec4(pos.xy, 0.0, 1.0);

	posV = pos; 
	uvV = uv;
	normV = norm;
}
)/shader";
}

constexpr const char* o2d_fs() {
	return R"/shader(
#version 330 core

in vec3 posV;
in vec2 uvV;
in vec3 normV;

out vec4 color;

uniform sampler2D texture0;

void main() {
	// texture sampling
	vec4 sample = texture(texture0, uvV);

	color = sample;
}

)/shader";
}

}
