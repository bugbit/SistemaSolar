#version 300 es
precision highp float;

#define PI radians(180.0)

// Model, View, Projection matrix.
uniform mat4 MVP;
uniform vec3 vCenter;
uniform float fA;
uniform float fB;
uniform int iNumVert;

void main() {
    float angle = (float(gl_VertexID) * 2.0f * PI) / float(iNumVert);
    //vec4 pos = vec4(vCenter, 1) + vec4(fA * sin(angle), 0, fB * cos(angle), 1);
    vec4 pos = vec4(vCenter + vec3(fA * sin(angle), 0, fB * cos(angle)), 1);
    //vec4 pos = vec4(vCenter.x + fA * sin(angle), vCenter.y, vCenter.z + fB * cos(angle), 1);
    gl_Position = MVP * pos;
    //gl_PointSize = 5.0;
}