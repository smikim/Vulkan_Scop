#include "Matrix.h"
#include "matrix.h"

#include "math.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void multi_matrix()
{
    // Mat4 클래스의 행렬 초기화
    mymath::Mat4 mat1;
    mymath::Mat4 mat2;

    // 예제 행렬 값 설정
    for (int i = 0; i < 16; ++i) {
        mat1._m[i] = static_cast<float>(i + 1);
        mat2._m[i] = static_cast<float>(16 - i);
    }

    // Mat4 클래스의 행렬 곱셈
    mymath::Mat4 resultMat4 = mat1 * mat2;

    // glm::mat4 클래스의 행렬 초기화
    glm::mat4 glmMat1 = glm::make_mat4(mat1._m);
    glm::mat4 glmMat2 = glm::make_mat4(mat2._m);

    // 결과 비교
    std::cout << "mat1 :" << std::endl;
    mat1.print();
    std::cout << "mat2 :" << std::endl;
    mat2.print();
    std::cout << "Mat4 결과:" << std::endl;
    resultMat4.print();


    //for (int i = 0; i < 4; ++i) {
    //    for (int j = 0; j < 4; ++j) {
    //        std::cout << resultMat4._m[i + j * 4] << " ";
    //    }
    //    std::cout << std::endl;
    //}

    std::cout << "glm::glmMat1:" << std::endl;
    const float* pSource = glm::value_ptr(glmMat1);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }
    std::cout << "glm::glmMat2:" << std::endl;
    pSource = glm::value_ptr(glmMat2);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }

    // glm::mat4 클래스의 행렬 곱셈
    glm::mat4 resultGlm = glmMat1 * glmMat2;

    std::cout << "glm::mat4 결과:" << std::endl;
    pSource = glm::value_ptr(resultGlm);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }
}

void compareMat4AndGLM() {
    // Mat4 클래스의 행렬 초기화
    mymath::Mat4 mat;
    for (int i = 0; i < 16; ++i) {
        mat._m[i] = static_cast<float>(i + 1);
    }

    // Vec3 클래스의 벡터 초기화
    mymath::Vec3 vec(1.0f, 2.0f, 3.0f);

    // Mat4 클래스의 행렬-벡터 곱셈
    mymath::Vec3 resultVec = mat * vec;

    // glm::mat4 클래스의 행렬 초기화
    glm::mat4 glmMat = glm::make_mat4(mat._m);

    // glm::vec3 클래스의 벡터 초기화
    glm::vec3 glmVec(1.0f, 2.0f, 3.0f);

    // glm::mat4 클래스의 행렬-벡터 곱셈
    glm::vec3 resultGlmVec = glmMat * glm::vec4(glmVec, 0.0f);

    // 결과 비교
    std::cout << "Mat4 결과:" << std::endl;
    std::cout << "x: " << resultVec._x << ", y: " << resultVec._y << ", z: " << resultVec._z << std::endl;

    std::cout << "glm::mat4 결과:" << std::endl;
    std::cout << "x: " << resultGlmVec.x << ", y: " << resultGlmVec.y << ", z: " << resultGlmVec.z << std::endl;
}

// glm::mat4 출력 함수
void printGLMMat4(const glm::mat4& mat) {
    const float* m = glm::value_ptr(mat);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            //std::cout << m[j * 4 + i] << " ";
            std::cout << m[i * 4 + j] << " ";
        }
        std::cout << std::endl;
    }
}

// 테스트 함수 정의
void testLookAt() {
    // 카메라 위치, 바라보는 위치, 위쪽 벡터 정의
    std::cout << "testLookAt:" << std::endl;
    mymath::Vec3 eye = { 2.0f, 2.0f, 2.0f };
    mymath::Vec3 center = { 0.0f, 0.0f, 0.0f };
    mymath::Vec3 up = { 0.0f, -1.0f, 0.0f };

    //ubo.viewMatrix = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //ubo.viewMatrix = mymath::lookAt(mymath::Vec3(2.0f, 2.0f, 2.0f), mymath::Vec3(0.0f, 0.0f, 0.0f), mymath::Vec3(0.0f, 0.0f, 1.0f));

    // Mat4 클래스의 lookAt 함수 호출
    //mymath::Mat4 mat4LookAt = mymath::lookAt(eye, center, up);
    

    mymath::Mat4 mat4LookAt = mymath::lookAtGLM(eye, center, up);

    // glm::lookAt 함수 호출
    glm::mat4 glmLookAt = glm::lookAt(glm::vec3(eye._x, eye._y, eye._z), glm::vec3(center._x, center._y, center._z), glm::vec3(up._x, up._y, up._z));

    std::cout << "glm::lookAt 결과:" << std::endl;
    const float* pSource1 = (const float*)glm::value_ptr(glmLookAt);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource1[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }

    // 결과 출력
    std::cout << "Mat4 lookAt 결과:" << std::endl;
    mat4LookAt.print();




    //std::cout << "glm::lookAt 결과:" << std::endl;
    //printGLMMat4(glmLookAt);
}

void testLookAt2() {
    std::cout << "testLookAt2:" << std::endl;
    // 카메라 위치, 바라보는 위치, 위쪽 벡터 정의
    mymath::Vec3 eye = { 1.0f, 2.0f, 3.0f };
    mymath::Vec3 center = { 4.0f, 5.0f, 6.0f };
    mymath::Vec3 up = { 0.0f, -1.0f, 0.0f };

    // Mat4 클래스의 lookAt 함수 호출
    mymath::Mat4 mat4LookAt = mymath::lookAt(eye, center, up);

    // glm::lookAt 함수 호출
    glm::mat4 glmLookAt = glm::lookAt(glm::vec3(eye._x, eye._y, eye._z), glm::vec3(center._x, center._y, center._z), glm::vec3(up._x, up._y, up._z));
   
    std::cout << "glm::lookAt 결과:" << std::endl;
    const float* pSource1 = (const float*)glm::value_ptr(glmLookAt);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource1[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }

    // 결과 출력
    std::cout << "Mat4 lookAt 결과:" << std::endl;
    mat4LookAt.print();

    std::cout << "glm::lookAt 결과:" << std::endl;
    //printGLMMat4(glmLookAt);
}

bool compareMatrices(const mymath::Mat4& customMat, const glm::mat4& glmMat) {
    const float* glmData = glm::value_ptr(glmMat);
    for (int i = 0; i < 16; ++i) {
        if (std::abs(customMat._m[i] - glmData[i]) > 1e-5) {
            return false;
        }
    }
    return true;
}

void comparePerspectiveFunctions() {
    float fov = glm::radians(45.0f);
    float aspectRatio = 1280.f / 720.f;
    float nearPlane = 0.1f;
    float farPlane = 10.0f;

    //mymath::Mat4 customPerspective = mymath::perspective(fov, aspectRatio, nearPlane, farPlane);
    mymath::Mat4 customPerspective = mymath::perspectiveGLM(fov, aspectRatio, nearPlane, farPlane);
    
    glm::mat4 glmPerspective = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

    std::cout << "Custom Perspective Matrix (Vulkan):" << std::endl;
    customPerspective.print();

    std::cout << "GLM Perspective Matrix (Vulkan) :" << std::endl;
    const float* pSource1 = (const float*)glm::value_ptr(glmPerspective);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource1[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }

    //std::cout << "GLM Perspective Matrix (Vulkan):" << std::endl;
    //printGLMMat4(glmPerspective);

    if (compareMatrices(customPerspective, glmPerspective)) {
        std::cout << "The matrices are identical." << std::endl;
    }
    else {
        std::cout << "The matrices are different." << std::endl;
    }
}

// 행렬 출력 함수
void printMatrix(const glm::mat4& mat) {
    const float* m = glm::value_ptr(mat);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << m[j * 4 + i] << " ";
        }
        std::cout << std::endl;
    }
}

// 회전 테스트 함수
void testRotationFunctions() {
    // 사용자 정의 회전 함수 테스트
    mymath::Mat4 customMat(1.0f);

    customMat.print();

    mymath::Vec3 axis(0.0f, 1.0f, 0.0f);
    
    //float angle = glm::radians(45.0f);
    float angle = mymath::radians(45.0f);
    
    mymath::Mat4 rotatedCustomMat = mymath::rotate(customMat, angle, axis);

    // glm 회전 함수 테스트
    glm::mat4 glmMat = glm::mat4(1.0f);
    glm::vec3 glmAxis(0.0f, 1.0f, 0.0f);

    glm::mat4 rotatedGlmMat = glm::rotate(glmMat, angle, glmAxis);

    // 결과 출력
    std::cout << "Custom Rotation Matrix:" << std::endl;
    rotatedCustomMat.print();

    std::cout << "GLM Rotation Matrix:" << std::endl;
    const float* pSource = glm::value_ptr(rotatedGlmMat);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }
}

// 회전 테스트 함수
void testRotationFunctions2() {
    // 사용자 정의 회전 함수 테스트
    mymath::Mat4 customMat(1.0f);
    mymath::Vec3 axis(0.0f, 1.0f, 0.0f); // Y축 기준
    float angle = glm::radians(90.0f); // 90도 회전
    mymath::Mat4 rotatedCustomMat = mymath::rotate(customMat, angle, axis);

    // glm 회전 함수 테스트
    glm::mat4 glmMat = glm::mat4(1.0f);
    glm::vec3 glmAxis(0.0f, 1.0f, 0.0f); // Y축 기준
    glm::mat4 rotatedGlmMat = glm::rotate(glmMat, angle, glmAxis);

    // 결과 출력
    std::cout << "Custom Rotation Matrix:" << std::endl;
    rotatedCustomMat.print();

    std::cout << "GLM Rotation Matrix:" << std::endl;
    const float* pSource = glm::value_ptr(rotatedGlmMat);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }
}

void testScaleFunctions() {
    // 사용자 정의 스케일 함수 테스트
    mymath::Mat4 customMat(1.0f);
    mymath::Vec3 scaleFactors(2.0f, 3.0f, 4.0f);
    mymath::Mat4 scaledCustomMat = mymath::scale(customMat, scaleFactors);

    // glm 스케일 함수 테스트
    glm::mat4 glmMat = glm::mat4(1.0f);
    glm::vec3 glmScaleFactors(2.0f, 3.0f, 4.0f);
    glm::mat4 scaledGlmMat = glm::scale(glmMat, glmScaleFactors);

    // 결과 출력
    std::cout << "Custom Scale Matrix:" << std::endl;
    scaledCustomMat.print();

    std::cout << "GLM Scale Matrix:" << std::endl;
    const float* pSource = glm::value_ptr(scaledGlmMat);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }
}

// 변환 테스트 함수
void testTranslateFunctions() {
    // 사용자 정의 변환 함수 테스트
    mymath::Mat4 customMat(1.0f);
    mymath::Vec3 translation(1.0f, 2.0f, 3.0f);
    mymath::Mat4 translatedCustomMat = mymath::translate(customMat, translation);

    // glm 변환 함수 테스트
    glm::mat4 glmMat = glm::mat4(1.0f);
    glm::vec3 glmTranslation(1.0f, 2.0f, 3.0f);
    glm::mat4 translatedGlmMat = glm::translate(glmMat, glmTranslation);

    // 결과 출력
    std::cout << "Custom Translation Matrix:" << std::endl;
    translatedCustomMat.print();

    std::cout << "GLM Translation Matrix:" << std::endl;
    const float* pSource = glm::value_ptr(translatedGlmMat);
    for (int i = 0; i < 16; ++i) {
        std::cout << pSource[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }
}
