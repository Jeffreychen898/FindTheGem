#version 420 core
varying vec2 v_texCoord;
varying vec4 v_color;

uniform sampler2D u_sampler;

void main()
{
	float alpha_value = texture(u_sampler, v_texCoord).r;
	float alpha = smoothstep(0.4, 0.5, alpha_value);//0.4, 0.5
	gl_FragColor = vec4(vec3(1.0), alpha) * v_color;
};