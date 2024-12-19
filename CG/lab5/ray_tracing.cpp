#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <cmath>

using namespace glm;

const int WIDTH = 800;
const int HEIGHT = 600;
const float MAX_REFLECTION_DEPTH = 3;

struct Ray {
    vec3 origin;
    vec3 direction;

    // Конструктор, который инициализирует обе переменные
    Ray(const vec3& orig, const vec3& dir) : origin(orig), direction(dir) {}
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

float intersectSphere(const Ray& ray, const Sphere& sphere) {
    vec3 oc = ray.origin - sphere.center;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return -1.0f;
    return (-b - sqrt(discriminant)) / (2.0f * a);
}

float intersectPlane(const Ray& ray, const Plane& plane) {
    float denom = dot(plane.normal, ray.direction);
    if (abs(denom) > 1e-6) {
        float t = dot(plane.point - ray.origin, plane.normal) / denom;
        return t > 0 ? t : -1.0f;
    }
    return -1.0f;
}

vec3 computeFog(const vec3& color, float distance, float fogDensity) {
    float fogFactor = exp(-fogDensity * distance);
    return mix(vec3(0.5f, 0.5f, 0.5f), color, fogFactor);
}

bool isInShadow(const Ray& ray, const Sphere& sphere1, const Sphere& sphere2, const Plane& plane, const Light& light) {
    // Тень проверяется через просылку луча от точки пересечения к источнику света
    float tSphere1 = intersectSphere(ray, sphere1);
    float tSphere2 = intersectSphere(ray, sphere2);
    float tPlane = intersectPlane(ray, plane);

    // Если пересечение найдено до источника света, то объект в тени
    if (tSphere1 > 0 || tSphere2 > 0 || tPlane > 0) {
        return true; // Тень
    }

    return false; // Нет тени
}

vec3 reflect(const vec3& I, const vec3& N) {
    return I - 2.0f * dot(I, N) * N; // Формула отражения
}

vec3 traceRay(const Ray& ray, const Sphere& sphere1, const Sphere& sphere2, const Plane& plane, const Light& light, float fogDensity, int depth) {
    if (depth > MAX_REFLECTION_DEPTH) return vec3(0.0f); // Ограничение глубины отражений

    float tSphere1 = intersectSphere(ray, sphere1);
    float tSphere2 = intersectSphere(ray, sphere2);
    float tPlane = intersectPlane(ray, plane);

    float t = tSphere1;
    vec3 hitColor = vec3(0.0f);
    vec3 hitNormal;
    vec3 hitPoint;

    if (tSphere1 > 0 && (tSphere1 < t || t < 0)) {
        hitPoint = ray.origin + tSphere1 * ray.direction;
        hitNormal = normalize(hitPoint - sphere1.center);
        vec3 lightDir = normalize(light.position - hitPoint);

        // Диффузное освещение
        float diff = max(dot(hitNormal, lightDir), 0.0f);
        hitColor = diff * light.color;

        t = tSphere1;
    }

    if (tSphere2 > 0 && (tSphere2 < t || t < 0)) {
        hitPoint = ray.origin + tSphere2 * ray.direction;
        hitNormal = normalize(hitPoint - sphere2.center);
        vec3 lightDir = normalize(light.position - hitPoint);

        // Диффузное освещение
        float diff = max(dot(hitNormal, lightDir), 0.0f);
        hitColor = diff * light.color;

        t = tSphere2;
    }

    if (tPlane > 0 && (tPlane < t || t < 0)) {
        hitPoint = ray.origin + tPlane * ray.direction;
        hitNormal = plane.normal;
        vec3 lightDir = normalize(light.position - hitPoint);

        // Диффузное освещение
        float diff = max(dot(hitNormal, lightDir), 0.0f);
        hitColor = diff * light.color;

        t = tPlane;
    }

    if (t > 0) {
        // Проверка на тень
        if (isInShadow(Ray(hitPoint, light.position - hitPoint), sphere1, sphere2, plane, light)) {
            hitColor *= 0.2f; // Уменьшаем интенсивность света для объекта в тени
        }

        // Отражение
        vec3 reflectedDir = reflect(ray.direction, hitNormal);
        Ray reflectedRay = { hitPoint + 0.001f * hitNormal, reflectedDir };
        vec3 reflectedColor = traceRay(reflectedRay, sphere1, sphere2, plane, light, fogDensity, depth + 1);
        hitColor = mix(hitColor, reflectedColor, 0.5f); // Смешиваем отражение с основным цветом

        return computeFog(hitColor, t, fogDensity);
    }

    return vec3(0.0f); // Фон
}


void renderScene(const Sphere& sphere1, const Sphere& sphere2, const Plane& plane, const Light& light, float fogDensity) {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            float u = (float(x) / WIDTH) * 2.0f - 1.0f;
            float v = (float(y) / HEIGHT) * 2.0f - 1.0f;
            vec3 rayDir = normalize(vec3(u, v, -1.0f));
            Ray ray = { vec3(0.0f, 0.0f, 0.0f), rayDir };
            vec3 color = traceRay(ray, sphere1, sphere2, plane, light, fogDensity, 0);

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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracing with Reflections, Shadows, and Lighting", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewInit();

    Sphere sphere1 = { vec3(-0.5f, 0.0f, -3.0f), 0.5f };
    Sphere sphere2 = { vec3(0.5f, 0.0f, -3.0f), 0.5f };
    Plane plane = { vec3(0.0f, -0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f) };
    Light light = { vec3(1.0f, 1.0f, -2.0f), vec3(1.0f, 1.0f, 1.0f) };

    float fogDensity = 0.1f;

    while (!glfwWindowShouldClose(window)) {
        renderScene(sphere1, sphere2, plane, light, fogDensity);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
