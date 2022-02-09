#version 300 es
precision highp float;
in vec4 vPosition; 

void main()
{
    gl_Position = vPosition;
}