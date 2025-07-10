#version 330

/*
A point to plot in non-clip coordinates
*/
layout (location = 0) in float y;
uniform uint numSamples;

/*
sampleMax and sampleMin are minimum and maximum values for the signals in normalized coordinates
*/
uniform float sampleMax = 1.0;
uniform float sampleMin = -1.0;

void main() {
    float x = gl_VertexID;

    float scale = 1 / sampleMax;
    float min = -1.0 - sampleMin;
    gl_Position = vec4((x * 2 / numSamples) - 1, scale * y - min, 0.0, 1.0);
}