#version 420 core
varying vec2 v_texCoord;
varying vec4 v_color;

uniform sampler2D u_sampler[2];

void main()
{
	gl_FragColor = texture(u_sampler[0], v_texCoord) + texture(u_sampler[1], v_texCoord);
};