#version 300 es
precision highp float;
in vec2 iTexcoord;
out vec4 outColor; 
uniform sampler2D tex;

void main()
{
    outColor = vtexture(tex,iTexcoord);
} 