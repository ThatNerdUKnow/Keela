#version 330

/*
A point to plot in non-clip coordinates
*/
layout (location = 0) in float y;
uniform uint numSamples;

/*
sampleMax and sampleMin are minimum and maximum values for the signals in normalized coordinates
*/
uniform float sampleMax = 255;
uniform float sampleMin = 0;

void main() {
    float x = gl_VertexID;
    float span = sampleMax - sampleMin;
    float scale = 1 / span;
    float offset = 0;

    // offset is applied to every y coord such that sampleMin will be transformed to 0.0
    offset = sampleMin >= 0 ? sampleMin : -sampleMin;
    //float min = -1.0 - sampleMin;
    gl_Position = vec4((x * 2 / numSamples) - 1, scale * y, 0.0, 1.0);
}