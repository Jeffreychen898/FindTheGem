#version 420 core

varying vec2 v_glowLocation;
varying vec4 v_color;
varying float v_size;

void main()
{
	float dist = distance(gl_FragCoord.xy, v_glowLocation);
    
    float percent = clamp(1.0 - dist / v_size, 0.0, 1.0);
    
    percent = percent * percent;
    
    gl_FragColor = v_color * vec4(vec3(1.0), percent );
};