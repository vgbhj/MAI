#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace glm;

const int WIDTH = 800;
const int HEIGHT = 600;

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Sphere {
    vec3 center;
    float radius;
};

struct Plane {
    vec3 point;
    vec3 normal;
};

struct Light {
    vec3 position;
    vec3 color;
};

// Функция пересечения луча и сферы
float intersectSphere(const Ray& ray, const Sphere& sphere) {
    vec3 oc = ray.origin - sphere.center;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return -1.0f;
    return (-b - sqrt(discriminant)) / (2.0f * a);
}

// Функция пересечения луча и плоскости
float intersectPlane(const Ray& ray, const Plane& plane) {
    float denom = dot(plane.normal, ray.direction);
    if (abs(denom) > 1e-6) {
        float t = dot(plane.point - ray.origin, plane.normal) / denom;
        return t > 0 ? t : -1.0f;
    }
    return -1.0f;
}

// Функция для вычисления эффекта тумана
vec3 computeFog(const vec3& color, float distance, float fogDensity) {
    float fogFactor = exp(-fogDensity * distance);  // Коэффициент тумана
    return mix(vec3(0.5f, 0.5f, 0.5f), color, fogFactor);  // Переход от фона к цвету
}

// Добавление объёмного рассеяния и поглощения света
vec3 computeVolumetricLighting(const Ray& ray, const vec3& hitPoint, const vec3& lightPos, float fogDensity, const vec3& volumetricColor) {
    float totalDistance = length(hitPoint - ray.origin);  // Расстояние, пройденное лучом
    float scattering = exp(-fogDensity * totalDistance);  // Моделирование рассеяния
    float absorption = exp(-fogDensity * totalDistance);  // Моделирование поглощения

    vec3 lightDir = normalize(lightPos - hitPoint);
    float diff = max(dot(lightDir, normalize(hitPoint - ray.origin)), 0.0f);
    
    // Смешиваем рассеянный и поглощённый свет с яркостью, умножив на цвет источника света
    return scattering * absorption * diff * volumetricColor; // Умножаем на цвет света (volumetricColor)
}

// Функция для проверки, находится ли точка в тени
bool isInShadow(const vec3& hitPoint, const Light& light, const Sphere& sphere1, const Sphere& sphere2, const Plane& plane) {
    const int numSamples = 16;  // Число сэмплов для мягкой тени
    float shadowFactor = 0.0f;

    // Генерация случайных направлений для каждого сэмпла
    for (int i = 0; i < numSamples; ++i) {
        vec3 randomOffset = vec3((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f);
        Ray shadowRay = { hitPoint, normalize(light.position + randomOffset - hitPoint) };
        
        // Проверка пересечений для каждого сэмпла
        float tSphere1 = intersectSphere(shadowRay, sphere1);
        float tSphere2 = intersectSphere(shadowRay, sphere2);
        float tPlane = intersectPlane(shadowRay, plane);

        if (tSphere1 > 0 || tSphere2 > 0 || tPlane > 0) {
            shadowFactor += 1.0f;
        }
    }

    shadowFactor /= numSamples;
    return shadowFactor > 0.5f;  // В зависимости от порога определяем, в тени ли точка
}

// Обновленная функция для вычисления освещенности
vec3 computeLighting(const Ray& ray, const vec3& hitPoint, const vec3& normal, const Light& light, bool inShadow, float distance, float fogDensity, const vec3& volumetricColor) {
    vec3 lightDir = normalize(light.position - hitPoint);
    float diff = max(dot(normal, lightDir), 0.0f);
    
    // Учитываем мягкость тени
    vec3 shadowEffect = inShadow ? vec3(0.1f) : vec3(1.0f);
    vec3 lightEffect = diff * light.color * shadowEffect;

    // Моделирование тумана
    float fogFactor = exp(-fogDensity * distance);
    return mix(vec3(0.5f, 0.5f, 0.5f), lightEffect, fogFactor) + computeVolumetricLighting(ray, hitPoint, light.position, fogDensity, volumetricColor);
}

// Обновленная функция трассировки луча с учетом теней
vec3 traceRay(const Ray& ray, const Sphere& sphere1, const Sphere& sphere2, const Plane& plane, const Light& light, float fogDensity, const vec3& volumetricColor) {
    float tSphere1 = intersectSphere(ray, sphere1);
    float tSphere2 = intersectSphere(ray, sphere2);
    float tPlane = intersectPlane(ray, plane);

    float t = tSphere1;
    vec3 hitColor = vec3(0.0f);

    if (tSphere1 > 0 && (tSphere1 < t || t < 0)) {
        vec3 hitPoint = ray.origin + tSphere1 * ray.direction;
        vec3 normal = normalize(hitPoint - sphere1.center);
        bool inShadow = isInShadow(hitPoint, light, sphere1, sphere2, plane);
        hitColor = computeLighting(ray, hitPoint, normal, light, inShadow, tSphere1, fogDensity, volumetricColor);
        t = tSphere1;
    }

    if (tSphere2 > 0 && (tSphere2 < t || t < 0)) {
        vec3 hitPoint = ray.origin + tSphere2 * ray.direction;
        vec3 normal = normalize(hitPoint - sphere2.center);
        bool inShadow = isInShadow(hitPoint, light, sphere1, sphere2, plane);
        hitColor = computeLighting(ray, hitPoint, normal, light, inShadow, tSphere2, fogDensity, volumetricColor);
        t = tSphere2;
    }

    if (tPlane > 0 && (tPlane < t || t < 0)) {
        vec3 hitPoint = ray.origin + tPlane * ray.direction;
        vec3 normal = plane.normal;
        bool inShadow = isInShadow(hitPoint, light, sphere1, sphere2, plane);
        hitColor = computeLighting(ray, hitPoint, normal, light, inShadow, tPlane, fogDensity, volumetricColor);
        t = tPlane;
    }

    if (t > 0) {
        return computeFog(hitColor, t, fogDensity);  // Добавление тумана
    }

    return vec3(0.0f); // Цвет фона
}

// Функция рендеринга сцены
void renderScene(const Sphere& sphere1, const Sphere& sphere2, const Plane& plane, const Light& light, float fogDensity, const vec3& volumetricColor) {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            float u = (float(x) / WIDTH) * 2.0f - 1.0f;
            float v = (float(y) / HEIGHT) * 2.0f - 1.0f;
            vec3 rayDir = normalize(vec3(u, v, -1.0f));
            Ray ray = { vec3(0.0f, 0.0f, 0.0f), rayDir };
            vec3 color = traceRay(ray, sphere1, sphere2, plane, light, fogDensity, volumetricColor);

            glColor3f(color.r, color.g, color.b);
            glVertex2f(u, v);
        }
    }
    glEnd();

    glfwSwapBuffers(glfwGetCurrentContext());
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracing with Shadows", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    Sphere sphere1 = { vec3(0.0f, 0.0f, -3.0f), 1.0f };
    Sphere sphere2 = { vec3(2.0f, 0.0f, -4.0f), 1.0f };
    Plane plane = { vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f) };
    Light light = { vec3(5.0f, 5.0f, -3.0f), vec3(1.0f, 1.0f, 1.0f) };
    float fogDensity = 0.5f;
    vec3 volumetricColor = vec3(1.0f, 1.0f, 1.0f);  // Начальный цвет объёмного света

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            fogDensity += 0.1f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            fogDensity -= 0.1f;
        }

        renderScene(sphere1, sphere2, plane, light, fogDensity, volumetricColor);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
