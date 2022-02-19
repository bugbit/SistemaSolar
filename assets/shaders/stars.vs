#version 300 es
precision highp float;
in vec4 vPosition; 
in vec2 iTexcoord;
out vec2 oTexcoord;

void main()
{
    gl_Position = vPosition;
    oTexcoord = iTexcoord;
}