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
        vec3(1.0, 0.0, 0.0), // ����
        vec3(1.0, 0.5, 0.0), // ��Ȳ
        vec3(1.0, 1.0, 0.0), // ���
        vec3(0.0, 1.0, 0.0), // �ʷ�
        vec3(0.0, 0.0, 1.0), // �Ķ�
        vec3(0.29, 0.0, 0.51), // ����
        vec3(0.56, 0.0, 1.0)  // ����
    );

    const vec3 grayscaleColors[7] = vec3[7](
        vec3(0.1, 0.1, 0.1), // ��ο� ȸ��
        vec3(0.2, 0.2, 0.2), // ���� �� ���� ȸ��
        vec3(0.3, 0.3, 0.3), // �߰� ȸ��
        vec3(0.4, 0.4, 0.4), // �߰� ���� ȸ��
        vec3(0.5, 0.5, 0.5), // ���� ȸ��
        vec3(0.6, 0.6, 0.6), // �� ���� ȸ��
        vec3(0.7, 0.7, 0.7)  // ���� ���� ȸ��
    );

     if (ubo.colorMode == 0) {
        // ���κ��� ����
        outColor = vec4(rainbowColors[fragTriangleID % 7], 1.0);
    } else if (ubo.colorMode == 1) {
        // 7���� �׷��̽����� ����
        outColor = vec4(grayscaleColors[fragTriangleID % 7], 1.0);
    } else if (ubo.colorMode == 2) {
        // �ؽ�ó��
        //outColor = texture(textureSampler, fragUV);
        outColor = vec4(texture(texSampler, fragTexCoord).rgb, 1.0);
    } else {
        // �⺻ ����
        outColor = vec4(fragColor, 1.0);
    }
     //outColor = vec4(fragTexCoord, 0.0, 1.0);
     //outColor = vec4(fragColor + texture(texSampler, fragTexCoord).rgb, 1.0);
     //outColor = vec4(texture(texSampler, fragTexCoord).rgb, 1.0);
}