#version 330 core

in vec2 texCoord;
in vec4 position;
out vec4 FragColor;

uniform sampler2D videoTexture;
uniform float heatmap_threshold_high = 1.0;
uniform float heatmap_threshold_low = 0.0;


uniform float heatmap_saturation = 1.0;
uniform float heatmap_lightness = 0.5;

// https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB_alternative
float f_k(float n, float H) {
    return mod((n + (H / 30)), 12);
}

float f_alpha() {
    return heatmap_saturation * min(heatmap_lightness, 1 - heatmap_lightness);
}

float f(float n, float H) {
    float a = f_alpha();
    float k = f_k(n, H);
    return heatmap_lightness - a * max(-1, min(min(k - 3.0, 9.0 - k), 1.0));
}

/// convert HSL color to rgb. H must be in the range [0,360]
/// S & L are taken as uniforms
vec3 hsl2rgb(float H) {
    return vec3(f(0, H), f(8, H), f(4, H));
}

/// END wikipedia HSL conversion functions

void main() {
    // for now blending texture with calculated color so I can see if shaders are broken
    vec4 texSample = vec4(texture(videoTexture, texCoord).xxx, 1.0);
    if (texSample.x >= heatmap_threshold_low && texSample.x <= heatmap_threshold_high) {
        // convert texSample into the range [0,360]. This is assuming that texSample.x is in the range [0,1]
        float H = texSample.x * 360;
        vec3 color = hsl2rgb(H);
        FragColor = vec4(color.xyz, 1.0);
        return;
    }
    FragColor = texSample;
}