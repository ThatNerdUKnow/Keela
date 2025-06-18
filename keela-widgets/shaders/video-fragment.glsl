#version 330 core

in vec2 texCoord;
in vec4 position;
out vec4 FragColor;

uniform sampler2D videoTexture;

void main() {
    // for now blending texture with calculated color so I can see if shaders are broken
    FragColor = position + texture(videoTexture, texCoord);
}