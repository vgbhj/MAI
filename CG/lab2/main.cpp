#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h> 
#include <iostream>

// Углы вращения
float rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
float rotationSpeed = 0.5f; // Скорость вращения
float cameraDistance = -3.0f; // Расстояние от камеры до куба
bool isPerspective = false; // Текущая проекция

void setupProjection(int width, int height, bool perspective) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (perspective) {
        gluPerspective(45.0, (double)width / height, 1.0, 500.0);
    } else {
        glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);
    }
    glMatrixMode(GL_MODELVIEW);
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
    sf::Window window(sf::VideoMode(800, 600), "3D Cube - Camera Movement", sf::Style::Default, sf::ContextSettings(24));
    window.setFramerateLimit(60);

    glEnable(GL_DEPTH_TEST);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P)
                    isPerspective = !isPerspective; // Переключение проекции
                if (event.key.code == sf::Keyboard::Up)
                    rotationSpeed += 0.1f; // Увеличение скорости
                if (event.key.code == sf::Keyboard::Down)
                    rotationSpeed -= 0.1f; // Уменьшение скорости
                if (event.key.code == sf::Keyboard::W)
                    cameraDistance += 0.1f; // Приближение куба
                if (event.key.code == sf::Keyboard::S)
                    cameraDistance -= 0.1f; // Удаление куба
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setupProjection(800, 600, isPerspective);

        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, cameraDistance); // Управляем расстоянием камеры

        rotationX += rotationSpeed;
        rotationY += rotationSpeed;
        rotationZ += rotationSpeed;

        glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
        glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);

        drawCube();

        window.display();
    }

    return 0;
}
