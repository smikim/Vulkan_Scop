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
        vec3(1.0, 0.0, 0.0), // 빨강
        vec3(1.0, 0.5, 0.0), // 주황
        vec3(1.0, 1.0, 0.0), // 노랑
        vec3(0.0, 1.0, 0.0), // 초록
        vec3(0.0, 0.0, 1.0), // 파랑
        vec3(0.29, 0.0, 0.51), // 남색
        vec3(0.56, 0.0, 1.0)  // 보라
    );

    const vec3 grayscaleColors[7] = vec3[7](
        vec3(0.1, 0.1, 0.1), // 어두운 회색
        vec3(0.2, 0.2, 0.2), // 조금 더 밝은 회색
        vec3(0.3, 0.3, 0.3), // 중간 회색
        vec3(0.4, 0.4, 0.4), // 중간 밝은 회색
        vec3(0.5, 0.5, 0.5), // 밝은 회색
        vec3(0.6, 0.6, 0.6), // 더 밝은 회색
        vec3(0.7, 0.7, 0.7)  // 가장 밝은 회색
    );

     if (ubo.colorMode == 0) {
        // 레인보우 색상
        outColor = vec4(rainbowColors[fragTriangleID % 7], 1.0);
    } else if (ubo.colorMode == 1) {
        // 7가지 그레이스케일 색상
        outColor = vec4(grayscaleColors[fragTriangleID % 7], 1.0);
    } else if (ubo.colorMode == 2) {
        // 텍스처링
        //outColor = texture(textureSampler, fragUV);
        outColor = vec4(texture(texSampler, fragTexCoord).rgb, 1.0);
    } else {
        // 기본 색상
        outColor = vec4(fragColor, 1.0);
    }
     //outColor = vec4(fragTexCoord, 0.0, 1.0);
     //outColor = vec4(fragColor + texture(texSampler, fragTexCoord).rgb, 1.0);
     //outColor = vec4(texture(texSampler, fragTexCoord).rgb, 1.0);
}