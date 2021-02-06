#version 420 core
varying vec2 v_texCoord;
varying vec4 v_color;

uniform sampler2D u_sampler;

void main()
{
	gl_FragColor = texture(u_sampler, v_texCoord) * v_color;
};