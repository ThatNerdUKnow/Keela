#version 330

/*
A point to plot in non-clip coordinates
*/
layout (location = 0) in float y;
uniform uint numSamples;

uniform float sampleMax = 255;
uniform float sampleMin = 0;

uniform float xoffset = 0;

void main() {
    float x = gl_VertexID;
    float max = sampleMax;
    float min = sampleMin;

    //[2025-07-15 22:56:06.815] [debug] GLTraceRender::on_gl_render: max: 146.1923, min: 105.80769
    float span = max - min; // 40.38461
    float scale = 1 / span; // .0247619081
    float yoffset = 0;

    // try to vertically center the plot if max == min
    if (max == min) {
        max += 0.1;
        min -= 0.1;
    }
    // offset is applied to every y coord such that sampleMin will be transformed to 0.0
    yoffset = min >= 0 ? min : -min;
    //float min = -1.0 - sampleMin;
    gl_Position = vec4(((x + xoffset) * 2 / numSamples) - 1, 2 * scale * (y - yoffset) - 1, 0.0, 1.0);
}