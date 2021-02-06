#version 420 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec4 color;

layout (std140, binding=0) uniform uniforms
{
	mat4 u_projection;
};

varying vec2 v_glowLocation;
varying vec4 v_color;
varying float v_size;

void main()
{
	v_color = color;
	v_glowLocation = texCoord;
	v_size = position.z;
	gl_Position = vec4(position.xy, 0.0, 1.0);
};