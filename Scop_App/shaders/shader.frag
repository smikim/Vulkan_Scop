#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) flat in uint fragTriangleID;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    uint colorMode;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

void main() {

    const vec3 rainbowColors[7] = vec3[7](
        vec3(1.0, 0.0, 0.0), 
        vec3(1.0, 0.5, 0.0), 
        vec3(1.0, 1.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0), 
        vec3(0.29, 0.0, 0.51), 
        vec3(0.56, 0.0, 1.0) 
    );

    const vec3 grayscaleColors[7] = vec3[7](
        vec3(0.1, 0.1, 0.1), 
        vec3(0.2, 0.2, 0.2), 
        vec3(0.3, 0.3, 0.3), 
        vec3(0.4, 0.4, 0.4), 
        vec3(0.5, 0.5, 0.5), 
        vec3(0.6, 0.6, 0.6), 
        vec3(0.7, 0.7, 0.7)  
    );

     if (ubo.colorMode == 0) {
        // 0: original
        outColor = vec4(fragColor, 1.0);
        
    } else if (ubo.colorMode == 1) {
        // 1: texture
        outColor = vec4(texture(texSampler, fragTexCoord).rgb, 1.0);
       
    } else if (ubo.colorMode == 2) {
        // 2: rainbow
        outColor = vec4(rainbowColors[fragTriangleID % 7], 1.0);
        
    } else {
        // 3: gray
        outColor = vec4(grayscaleColors[fragTriangleID % 7], 1.0); 
    }
}