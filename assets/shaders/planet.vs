#version 300 es
precision highp float;
layout(location=0) in vec3 iPosition; 
out vec2 oTexcoord;

// Model, View, Projection matrix.
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(iPosition,1);
    oTexcoord = iTexcoord;
}