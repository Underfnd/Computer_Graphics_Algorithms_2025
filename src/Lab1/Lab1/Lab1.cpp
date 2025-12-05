#define _USE_MATH_DEFINES
#include <Windows.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glu32.lib")

// Функция для рисования звезды с помощью GL_TRIANGLE_FAN
void drawStarFan(float x, float y, int starPoints, float R, float r) {
    int i;
    double angleStep = 2.0 * M_PI / starPoints;
    double angle = 0.0;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);  // Центр звезды
    
    for (i = 0; i < starPoints; i++) {
        glVertex2f(x + R * cos(angle), y + R * sin(angle));
        angle += angleStep;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
        angle += angleStep;
    }
    
    // Замыкаем фигуру
    angle = 0;
    glVertex2f(x + R * cos(angle), y + R * sin(angle));
    glEnd();
}

// Функция для рисования звезды с помощью GL_TRIANGLE_LIST
void drawStarList(float x, float y, int starPoints, float R, float r) {
    int i;
    double angleStep = 2.0 * M_PI / starPoints;
    double angle = 0.0;

    glBegin(GL_TRIANGLES);
    for (i = 0; i < starPoints; i++) {
        // Первый треугольник луча
        glVertex2f(x, y);  // Центр
        glVertex2f(x + R * cos(angle), y + R * sin(angle));
        angle += angleStep;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
        
        // Второй треугольник луча
        glVertex2f(x, y);  // Центр
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
        angle += angleStep;
        glVertex2f(x + R * cos(angle), y + R * sin(angle));
    }
    glEnd();
}

// Функция для рисования звезды с помощью GL_TRIANGLE_STRIP
void drawStarStrip(float x, float y, int starPoints, float R, float r) {
    int i;
    double angleStep = 2.0 * M_PI / starPoints;
    double angle = 0.0;

    glBegin(GL_TRIANGLE_STRIP);
    
    // Начинаем с центра
    glVertex2f(x, y);
    
    for (i = 0; i <= starPoints; i++) {
        // Внешняя точка
        glVertex2f(x + R * cos(angle), y + R * sin(angle));
        
        // Внутренняя точка
        angle += angleStep;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
        
        angle += angleStep;
        
        // Возвращаемся к центру для следующего треугольника
        if (i < starPoints - 1) {
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Устанавливаем цвет звезд (желтый)
    glColor3f(1.0f, 1.0f, 0.0f);

    // 1. Звезда с 4 лучами - Triangle Fan
    drawStarFan(-0.6f, 0.6f, 4, 0.25f, 0.1f);
    
    // 2. Звезда с 6 лучами - Triangle List
    drawStarList(0.6f, 0.6f, 6, 0.25f, 0.1f);
    
    // 3. Звезда с 5 лучами - Triangle Strip
    drawStarStrip(0.0f, -0.3f, 5, 0.3f, 0.12f);
    
    // Дополнительные звезды для демонстрации
    glColor3f(0.0f, 1.0f, 1.0f);  // Голубой
    drawStarFan(-0.8f, -0.6f, 5, 0.2f, 0.08f);
    
    glColor3f(1.0f, 0.0f, 1.0f);  // Фиолетовый
    drawStarList(0.8f, -0.6f, 4, 0.2f, 0.08f);

    glutSwapBuffers();
}

void init() {
    // Изменяем цвет фона на темно-зеленый (по заданию)
    glClearColor(0.0f, 0.2f, 0.1f, 0.0f);
    
    // Настройка проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:  // ESC - выход
        exit(0);
        break;
    case 'r':
    case 'R':
        // Перерисовка
        glutPostRedisplay();
        break;
    }
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    // Инициализация GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Computer Graphics Lab 1 - Stars with different primitives");
    
    // Инициализация
    init();
    
    // Регистрация callback функций
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    
    // Главный цикл
    glutMainLoop();
    
    return 0;
}