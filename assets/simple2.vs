#version 300 es
precision highp float;
in vec4 vPosition; 
layout(location=8) in vec2 iTexcoord;
out vec2 oTexcoord;

void main()
{
    gl_Position = vPosition;
    oTexcoord = iTexcoord;
}