#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <iostream>

// Камера
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 5.0f; // Позиция камеры
float targetX = 0.0f, targetY = 0.0f, targetZ = 0.0f; // Точка, на которую смотрит камера
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;             // Вектор "вверх"

// Объект
float rotationAngle = 0.0f; // Угол вращения
float rotationAxisX = 0.0f; // Компонента вращения вокруг X
float rotationAxisY = 1.0f; // Компонента вращения вокруг Y (по умолчанию)
float rotationAxisZ = 0.0f; // Компонента вращения вокруг Z
bool rotateAroundCamera = false; // Режим вращения: вокруг камеры или центра объекта

void setupProjection(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void setupCamera() {
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, targetX, targetY, targetZ, upX, upY, upZ);
}

void drawCube() {
    glBegin(GL_QUADS);
    // Передняя грань
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Задняя грань
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Верхняя грань
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Нижняя грань
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);

    // Левая грань
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);

    // Правая грань
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glEnd();
}

int main() {
    sf::Window window(sf::VideoMode(800, 600), "3D Transformations - Rotation Control", sf::Style::Default, sf::ContextSettings(24));
    window.setFramerateLimit(60);

    glEnable(GL_DEPTH_TEST);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R)
                    rotateAroundCamera = !rotateAroundCamera; // Переключение режима вращения

                // Управление осями вращения
                if (event.key.code == sf::Keyboard::Num1)
                    rotationAxisX = 1.0f; // Включить вращение вокруг X
                if (event.key.code == sf::Keyboard::Num2)
                    rotationAxisX = 0.0f; // Выключить вращение вокруг X

                if (event.key.code == sf::Keyboard::Num3)
                    rotationAxisY = 1.0f; // Включить вращение вокруг Y
                if (event.key.code == sf::Keyboard::Num4)
                    rotationAxisY = 0.0f; // Выключить вращение вокруг Y

                if (event.key.code == sf::Keyboard::Num5)
                    rotationAxisZ = 1.0f; // Включить вращение вокруг Z
                if (event.key.code == sf::Keyboard::Num6)
                    rotationAxisZ = 0.0f; // Выключить вращение вокруг Z
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setupProjection(800, 600);
        setupCamera();

        // Плавное вращение будет происходить только если хотя бы одна ось вращения включена
        if (rotationAxisX != 0.0f || rotationAxisY != 0.0f || rotationAxisZ != 0.0f) {
            rotationAngle += 1.0f; // Плавное вращение
        }

        if (rotateAroundCamera) {
            glPushMatrix();
            glTranslatef(cameraX, cameraY, cameraZ - 2.0f); // Вращение вокруг точки камеры
            glRotatef(rotationAngle, rotationAxisX, rotationAxisY, rotationAxisZ);
            glTranslatef(-cameraX, -cameraY, -(cameraZ - 2.0f));
            drawCube();
            glPopMatrix();
        } else {
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, -2.0f); // Вращение вокруг центра объекта
            glRotatef(rotationAngle, rotationAxisX, rotationAxisY, rotationAxisZ);
            drawCube();
            glPopMatrix();
        }

        window.display();
    }

    return 0;
}
