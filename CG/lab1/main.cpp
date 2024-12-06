#include <SFML/Graphics.hpp>
#include <vector>
#include <random> 

// Константы для определения региона (Коэн-Сазерленд)
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Функция для определения региона точки
int computeOutCode(float x, float y, sf::FloatRect clipWindow) {
    int code = INSIDE;

    if (x < clipWindow.left)
        code |= LEFT;
    else if (x > clipWindow.left + clipWindow.width)
        code |= RIGHT;
    if (y < clipWindow.top)
        code |= TOP;
    else if (y > clipWindow.top + clipWindow.height)
        code |= BOTTOM;

    return code;
}

// Алгоритм Коэна-Сазерленда для отсечения линии
bool cohenSutherlandLineClip(float &x0, float &y0, float &x1, float &y1, sf::FloatRect clipWindow) {
    int outCode0 = computeOutCode(x0, y0, clipWindow);
    int outCode1 = computeOutCode(x1, y1, clipWindow);
    bool accept = false;

    while (true) {
        if (!(outCode0 | outCode1)) { // Оба конца внутри
            accept = true;
            break;
        } else if (outCode0 & outCode1) { // Оба конца снаружи в одном регионе
            break;
        } else {
            float x, y;

            int outCodeOut = outCode0 ? outCode0 : outCode1;

            // Вычисление пересечения
            if (outCodeOut & TOP) {
                x = x0 + (x1 - x0) * (clipWindow.top - y0) / (y1 - y0);
                y = clipWindow.top;
            } else if (outCodeOut & BOTTOM) {
                x = x0 + (x1 - x0) * ((clipWindow.top + clipWindow.height) - y0) / (y1 - y0);
                y = clipWindow.top + clipWindow.height;
            } else if (outCodeOut & RIGHT) {
                y = y0 + (y1 - y0) * ((clipWindow.left + clipWindow.width) - x0) / (x1 - x0);
                x = clipWindow.left + clipWindow.width;
            } else if (outCodeOut & LEFT) {
                y = y0 + (y1 - y0) * (clipWindow.left - x0) / (x1 - x0);
                x = clipWindow.left;
            }

            // Заменяем точку снаружи на точку пересечения
            if (outCodeOut == outCode0) {
                x0 = x;
                y0 = y;
                outCode0 = computeOutCode(x0, y0, clipWindow);
            } else {
                x1 = x;
                y1 = y;
                outCode1 = computeOutCode(x1, y1, clipWindow);
            }
        }
    }
    return accept;
}

// Генерация случайных чисел в диапазоне
float getRandomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

// Генерация случайных линий
std::vector<std::pair<sf::Vector2f, sf::Vector2f>> generateRandomLines(int count, float xMin, float xMax, float yMin, float yMax) {
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> lines;
    for (int i = 0; i < count; ++i) {
        sf::Vector2f start(getRandomFloat(xMin, xMax), getRandomFloat(yMin, yMax));
        sf::Vector2f end(getRandomFloat(xMin, xMax), getRandomFloat(yMin, yMax));
        lines.emplace_back(start, end);
    }
    return lines;
}

int main() {
    // Создаем окно SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Cohen-Sutherland Line Clipping");
    window.setFramerateLimit(60);

    // Параметры окна отсечения
    sf::FloatRect clipWindow(200, 150, 400, 300);
    sf::RectangleShape clipRectangle(sf::Vector2f(clipWindow.width, clipWindow.height));
    clipRectangle.setPosition(clipWindow.left, clipWindow.top);
    clipRectangle.setFillColor(sf::Color::Transparent);
    clipRectangle.setOutlineColor(sf::Color::Red);
    clipRectangle.setOutlineThickness(2);

    // Генерация случайных линий
    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> lineCoords = generateRandomLines(5, 50, 750, 50, 550);

    // Линии для отрисовки
    std::vector<sf::VertexArray> lines(lineCoords.size(), sf::VertexArray(sf::Lines, 2));

    // Основной цикл
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Управление окном отсечения (перемещение)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                clipWindow.left -= 2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                clipWindow.left += 2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                clipWindow.top -= 2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                clipWindow.top += 2;
            }

            // Масштабирование окна отсечения
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
                clipWindow.width += 2;
                clipWindow.height += 2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
                clipWindow.width -= 2;
                clipWindow.height -= 2;
            }

            // Обновление прямоугольника отсечения
            clipRectangle.setPosition(clipWindow.left, clipWindow.top);
            clipRectangle.setSize(sf::Vector2f(clipWindow.width, clipWindow.height));
        }

        // Отрисовка
        window.clear(sf::Color::Black);

        // Отрисовываем линии (до отсечения)
        for (size_t i = 0; i < lineCoords.size(); ++i) {
            float x0 = lineCoords[i].first.x, y0 = lineCoords[i].first.y;
            float x1 = lineCoords[i].second.x, y1 = lineCoords[i].second.y;

            if (cohenSutherlandLineClip(x0, y0, x1, y1, clipWindow)) {
                lines[i][0].position = sf::Vector2f(x0, y0);
                lines[i][1].position = sf::Vector2f(x1, y1);
                lines[i][0].color = sf::Color::Green;
                lines[i][1].color = sf::Color::Green;

                window.draw(lines[i]);
            }
        }

        // Отрисовываем окно отсечения
        window.draw(clipRectangle);

        window.display();
    }

    return 0;
}
