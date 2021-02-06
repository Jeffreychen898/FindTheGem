#version 420 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec4 color;

layout (std140, binding=0) uniform uniforms
{
	mat4 u_projection;
};

varying vec2 v_texCoord;
varying vec4 v_color;

void main()
{
	v_color = color;
	v_texCoord = texCoord;
	gl_Position = u_projection * vec4(position, 1.0);
};