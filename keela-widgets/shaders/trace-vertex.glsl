#version 330

/*
A point to plot in non-clip coordinates
*/
layout (location = 0) in vec2 point;
uniform uint numSamples;

/*
sampleMax and sampleMin are minimum and maximum values for the signals in normalized coordinates
*/
uniform float sampleMax;
uniform float sampleMin;

void main() {
    float scale = sampleMax - sampleMin;
    gl_Position = vec4(float(point.x) / numSamples, scale * point.y + sampleMin, 0.0, 1.0);
}