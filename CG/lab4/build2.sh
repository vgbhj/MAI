#!/bin/bash

# Имя исполняемого файла
OUTPUT="phong_lighting"

# Путь к исходному файлу
SOURCE="phong_lighting.cpp"

# Линковка библиотек
LIBS="-lsfml-window -lsfml-system -lGL -lGLEW"

# Компиляция
echo "Компилируем $SOURCE..."
g++ $SOURCE $LIBS -o $OUTPUT

# Проверяем успешность компиляции
if [ $? -eq 0 ]; then
    echo "Успешно скомпилировано: $OUTPUT"
    echo "Запуск приложения..."
    ./$OUTPUT
else
    echo "Ошибка компиляции!"
fi
