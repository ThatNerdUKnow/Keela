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
uniform float sampleMin = 0;

void main() {
    // confine min and max to be within the range 0-1 inclusive
    float max = clamp(sampleMax, 0, 1);
    float min = clamp(sampleMin, 0, max);

    float scale = max - min;
    gl_Position = vec4((float(point.x) * 2 / numSamples) - 1, scale * point.y + min, 0.0, 1.0);
}