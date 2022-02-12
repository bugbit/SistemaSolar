#version 300 es
precision highp float;
in vec4 vPosition; 
in vec2 in_texcoord;

void main()
{
    gl_Position = vPosition;
}