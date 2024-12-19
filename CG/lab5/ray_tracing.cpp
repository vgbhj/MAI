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

vec3 traceRay(const Ray& ray, const Sphere& sphere1, const Sphere& sphere2, const Plane& plane, const Light& light, float fogDensity) {
    float tSphere1 = intersectSphere(ray, sphere1);
    float tSphere2 = intersectSphere(ray, sphere2);
    float tPlane = intersectPlane(ray, plane);

    float t = tSphere1;
    vec3 hitColor = vec3(0.0f);

    if (tSphere1 > 0 && (tSphere1 < t || t < 0)) {
        vec3 hitPoint = ray.origin + tSphere1 * ray.direction;
        vec3 normal = normalize(hitPoint - sphere1.center);
        vec3 lightDir = normalize(light.position - hitPoint);
        float diff = max(dot(normal, lightDir), 0.0f);
        hitColor = diff * light.color;
        t = tSphere1;
    }

    if (tSphere2 > 0 && (tSphere2 < t || t < 0)) {
        vec3 hitPoint = ray.origin + tSphere2 * ray.direction;
        vec3 normal = normalize(hitPoint - sphere2.center);
        vec3 lightDir = normalize(light.position - hitPoint);
        float diff = max(dot(normal, lightDir), 0.0f);
        hitColor = diff * light.color;
        t = tSphere2;
    }

    if (tPlane > 0 && (tPlane < t || t < 0)) {
        vec3 hitPoint = ray.origin + tPlane * ray.direction;
        vec3 normal = plane.normal;
        vec3 lightDir = normalize(light.position - hitPoint);
        float diff = max(dot(normal, lightDir), 0.0f);
        hitColor = diff * light.color;
        t = tPlane;
    }

    if (t > 0) {
        return computeFog(hitColor, t, fogDensity);
    }

    return vec3(0.0f); // Background color
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
            vec3 color = traceRay(ray, sphere1, sphere2, plane, light, fogDensity);

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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracing with Volumetric Light", nullptr, nullptr);
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
