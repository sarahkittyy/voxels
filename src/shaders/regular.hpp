#pragma once

namespace shaders {

constexpr const char* regular_vs() {
	return R"/shader(
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 norm;

out vec3 posV;
out vec2 uvV;
out vec3 normV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
	mat4 mvp = proj * view * model;
	gl_Position = mvp * vec4(pos, 1.0);

	posV = vec3(model * vec4(pos, 1.0));
	uvV = uv;
	normV = norm;
}
)/shader";
}

constexpr const char* regular_fs() {
	return R"/shader(
#version 330 core

in vec3 posV;
in vec2 uvV;
in vec3 normV;

out vec4 color;

uniform sampler2D texture0;

uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;

void main() {
	// texture sampling
	vec4 sample = texture(texture0, uvV);
	
	// invert normal for lighting to work on scaled frags
	mat3 modelInv = mat3(transpose(inverse(model)));
	vec3 normInv = modelInv * normV;
	
	// variable setup
	vec3 camDir = normalize(camPos - posV);
	vec3 norm = normalize(normInv);
	vec3 lightDir = normalize(lightPos - posV);
	vec3 reflectDir = reflect(-lightDir, norm);

	// ambient light
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse light
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular lighting
	float specularStrength = 0.5;
	float spec = pow(max(dot(camDir, reflectDir), 0.0), 2);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = diffuse + ambient + specular;

	color = vec4(result, 1.0) * sample;
}

)/shader";
}

}
