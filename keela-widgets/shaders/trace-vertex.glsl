#version 330

/*
A point to plot in non-clip coordinates
*/
layout (location = 0) in vec2 point;
uniform uint numSamples;

/*
sampleMax and sampleMin are minimum and maximum values for the signals in normalized coordinates
*/
uniform float sampleMax = 1.0;
uniform float sampleMin = -1.0;

void main() {
    float scale = sampleMax - sampleMin;
    gl_Position = vec4((float(point.x) * 2 / numSamples) - 1, scale * point.y + sampleMin, 0.0, 1.0);
}