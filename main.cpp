#define LOG_LEVEL_DEBUG
#include "LOG.h"

#define RAYGUI_IMPLEMENTATION
#include "include/raylib.h"
#include "include/raygui.h"

#include <string>
#include <vector>
#include <iostream>

#include "V2.h"
#include "circle.h"

#define WIDTH 1200
#define HEIGHT 800
#define TITLE "2D Collisions (Discrete)"
#define UNIT_PER_PIXEL 5
#define FPS 100

#define UNIT_HEIGHT HEIGHT/UNIT_PER_PIXEL
#define UNIT_WIDTH WIDTH/UNIT_PER_PIXEL
#define DELTA_T 1/FPS

const int kBorderThickness = 5;
const int kCircleMaxSpeed = 20;
const int kCircleMaxMass = 10;
const int kCircleMaxCount = 200;

std::vector<Circle*> AllCircles = {};
bool isPaused = false;
float GUIe = 1;
int GUISpeed = 1;
int GUIMass = 1;
int GUIColor = 0;
Color colorList[5] = {
    RED,
    BLUE,
    GREEN,
    ORANGE,
    PURPLE
};

std::string GUIeText = "";
std::string GUISpeedText = "";
std::string GUIMassText = "";


void CheckBoundaryCollision(Circle* circle) {
    if (circle->position.x < -(UNIT_WIDTH / 2) + (kBorderThickness / UNIT_PER_PIXEL) + circle->radius) {
        circle->position.x = -(UNIT_WIDTH / 2) + (kBorderThickness / UNIT_PER_PIXEL) + circle->radius;
        circle->velocity.x = -circle->velocity.x;
    }
    else if (circle->position.x > (UNIT_WIDTH / 2) - (kBorderThickness / UNIT_PER_PIXEL) - circle->radius) {
        circle->position.x = (UNIT_WIDTH / 2) - (kBorderThickness / UNIT_PER_PIXEL) - circle->radius;
        circle->velocity.x = -circle->velocity.x;
    }
    if (circle->position.y < -(UNIT_HEIGHT / 2) + (kBorderThickness / UNIT_PER_PIXEL) + circle->radius) {
        circle->position.y = -(UNIT_HEIGHT / 2) + (kBorderThickness / UNIT_PER_PIXEL) + circle->radius;
        circle->velocity.y = -circle->velocity.y;
    }
    else if (circle->position.y > (UNIT_HEIGHT / 2) - (kBorderThickness / UNIT_PER_PIXEL) - circle->radius) {
        circle->position.y = (UNIT_HEIGHT / 2) - (kBorderThickness / UNIT_PER_PIXEL) - circle->radius;
        circle->velocity.y = -circle->velocity.y;
    }
}

void CollisionResponse(Circle* first, Circle* second) {
    //Displace/Static Response
    float deltaX = first->radius + second->radius - V2::Distance(first->position, second->position);
    first->position = first->position - (V2::Normalized(second->position - first->position) * (deltaX / 2.f));
    second->position = second->position - (V2::Normalized(first->position - second->position) * (deltaX / 2.f));
    //Dynamic Response
    V2 normal = V2::Normalized((second->position - first->position));
    float impulse = V2::Dot((first->velocity - second->velocity), normal) * (GUIe + 1) * (first->mass * second->mass) / (first->mass + second->mass);
    first->velocity = first->velocity - normal * (impulse / first->mass);
    second->velocity = second->velocity + normal * (impulse / second->mass);
}

bool CheckCollision(Circle* first, Circle* second) {
    return (pow(second->position.x - first->position.x, 2) + pow(second->position.y - first->position.y, 2) <= pow(first->radius + second->radius, 2));
}

void CheckCollisionsInVector(std::vector<Circle*>& vector) {
    for (int i = 0; i < vector.size(); i++) {
        for (int j = i + 1; j < vector.size(); j++) {
            if (CheckCollision(vector.at(i), vector.at(j))) {
                CollisionResponse(vector.at(i), vector.at(j));
            }
        }
    }
}

void MoveCircle(Circle* circle) {
    circle->position.x += circle->velocity.x * DELTA_T;
    circle->position.y += circle->velocity.y * DELTA_T;
}

bool AddCircle(Color color, float mass, float speed) {
    if (AllCircles.size() < kCircleMaxCount) {
        float radius = sqrt(mass);
        float xVelocity = ((float)rand() / ((float)RAND_MAX / (float)speed)) - speed;
        float yVelocity = sqrt(pow(speed, 2) - pow(xVelocity, 2));
        if (rand() % 2 == 1) {
            yVelocity = -yVelocity;
        }
        V2 velocity = V2(xVelocity, yVelocity);
        for (int i = 0; i < 10; i++) {
            int positionX = (rand() % (int)floorf((UNIT_WIDTH - ((radius - 2 - (kBorderThickness / UNIT_PER_PIXEL)) * 2))) - ((UNIT_WIDTH / 2) - ((radius - 2 - (kBorderThickness / UNIT_PER_PIXEL)))));
            int positionY = (rand() % (int)floorf((UNIT_HEIGHT - ((radius - 2 - (kBorderThickness / UNIT_PER_PIXEL)) * 2))) - ((UNIT_HEIGHT / 2) - ((radius - 2 - (kBorderThickness / UNIT_PER_PIXEL)))));
            Circle* candidateCircle = new Circle(radius, V2(positionX, positionY));
            bool isValid = true;
            for (Circle* circle : AllCircles) {
                if (CheckCollision(candidateCircle, circle)) {
                    isValid = false;
                    delete candidateCircle;
                    break;
                }
            }
            if (isValid) {
                AllCircles.push_back(candidateCircle);
                candidateCircle->color = color;
                candidateCircle->mass = mass;
                candidateCircle->velocity = velocity;
                return true;
            }
        }
    }
    return false;
}

bool AddRandomCircle() {
    Color color = colorList[rand() % 5];
    int mass = (rand() % kCircleMaxMass) + 1;
    float speed = (float)rand() / ((float)RAND_MAX / ((float)kCircleMaxSpeed - 5)) + 5;
    return AddCircle(color, mass, speed);
}

void DrawInfo(int posX, int posY) {
    std::string numberText = "Total number of particles: ";
    numberText.append(std::to_string(AllCircles.size()));
    DrawText(numberText.c_str(), posX, posY - 20, 10, GRAY);

    std::string averageEnergyText = "Average kinetic energy of the particles: ";
    float totalEnergy = 0.f;
    float averageEnergy = 0.f;
    if (!AllCircles.empty()) {
        for (Circle* circle : AllCircles) {
            totalEnergy += circle->mass * pow(circle->velocity.Length(), 2);
        }
        averageEnergy = totalEnergy / AllCircles.size();
    }
    averageEnergyText.append(std::to_string((int)averageEnergy));
    DrawText(averageEnergyText.c_str(), posX, posY - 40, 10, GRAY);

    int verticalOffset = 60;
    for (int i = 10; i >= 1; i--) {
        std::vector<Circle*> currentMass;
        for (Circle* circle : AllCircles) {
            if (circle->mass == i) {
                currentMass.push_back(circle);
            }
        }
        if (!currentMass.empty()) {
            averageEnergyText = "Average kinetic energy of the particles with mass ";
            averageEnergyText.append(std::to_string(i));
            averageEnergyText.append(": ");
            totalEnergy = 0.f;
            averageEnergy = 0.f;
            for (Circle* circle : currentMass) {
                totalEnergy += circle->mass * pow(circle->velocity.Length(), 2);
            }
            averageEnergy = totalEnergy / currentMass.size();
            averageEnergyText.append(std::to_string((int)averageEnergy));
            DrawText(averageEnergyText.c_str(), posX, posY - verticalOffset, 10, GRAY);
            verticalOffset += 20;
        }
    }
}

void DrawGUIBackground() {
    if (isPaused) {
        DrawText("PAUSED", WIDTH / 2 - 410, HEIGHT / 2 - 90, 200, LIGHTGRAY);
    }
}

void DrawCircles() {
    for (Circle* circle : AllCircles) {
        DrawCircle(WIDTH / 2 + circle->position.x * UNIT_PER_PIXEL, HEIGHT / 2 + circle->position.y * UNIT_PER_PIXEL, circle->radius * UNIT_PER_PIXEL, circle->color);
        DrawCircle(WIDTH / 2 + circle->position.x * UNIT_PER_PIXEL, HEIGHT / 2 + circle->position.y * UNIT_PER_PIXEL, circle->radius * UNIT_PER_PIXEL - UNIT_PER_PIXEL / 5, GRAY);
        DrawCircle(WIDTH / 2 + circle->position.x * UNIT_PER_PIXEL, HEIGHT / 2 + circle->position.y * UNIT_PER_PIXEL, UNIT_PER_PIXEL / 5, BLACK);
    }
}

void DrawBorders() {
    DrawRectangle(0, 0, kBorderThickness, HEIGHT, GRAY);
    DrawRectangle(WIDTH - kBorderThickness, 0, kBorderThickness, HEIGHT, GRAY);
    DrawRectangle(0, 0, WIDTH, kBorderThickness, GRAY);
    DrawRectangle(0, HEIGHT - kBorderThickness, WIDTH, kBorderThickness, GRAY);
}

void DrawGUIForeground() {
    GUIMass = GuiSlider(Rectangle{ 60, 15, 110, 15 }, "Mass  ", GUIMassText.c_str(), GUIMass, 1, kCircleMaxMass);
    GUIMassText = std::to_string(GUIMass);

    GUISpeed = GuiSlider(Rectangle{ 60, 35, 110, 15 }, "Speed  ", GUISpeedText.c_str(), GUISpeed, 0, kCircleMaxSpeed);
    GUISpeedText = std::to_string(GUISpeed);

    DrawRectangle(60, 53, 19, 19, RED);
    DrawRectangle(63, 56, 13, 13, WHITE);
    if (GuiCheckBox(Rectangle{ 62, 55, 15, 15 }, NULL, GUIColor == 0)) GUIColor = 0;
    DrawRectangle(83, 53, 19, 19, BLUE);
    DrawRectangle(86, 56, 13, 13, WHITE);
    if (GuiCheckBox(Rectangle{ 85, 55, 15, 15 }, NULL, GUIColor == 1)) GUIColor = 1;
    DrawRectangle(106, 53, 19, 19, GREEN);
    DrawRectangle(109, 56, 13, 13, WHITE);
    if (GuiCheckBox(Rectangle{ 108, 55, 15, 15 }, NULL, GUIColor == 2)) GUIColor = 2;
    DrawRectangle(128, 53, 19, 19, ORANGE);
    DrawRectangle(131, 56, 13, 13, WHITE);
    if (GuiCheckBox(Rectangle{ 130, 55, 15, 15 }, NULL, GUIColor == 3)) GUIColor = 3;
    DrawRectangle(151, 53, 19, 19, PURPLE);
    DrawRectangle(154, 56, 13, 13, WHITE);
    if (GuiCheckBox(Rectangle{ 153, 55, 15, 15 }, NULL, GUIColor == 4)) GUIColor = 4;
    if (GuiButton(Rectangle{ 60, 75, 110, 15 }, "Add Circle")) {
        AddCircle(colorList[GUIColor], GUIMass, GUISpeed);
    }

    if (GuiButton(Rectangle{ 60, 95, 110, 15 }, "Add 10 Circles")) {
        for (int i = 0; i < 10; i++) {
            AddCircle(colorList[GUIColor], GUIMass, GUISpeed);
        }
    }

    if (GuiButton(Rectangle{ 60, 135, 110, 15}, "Add Rnd. Circle")) {
        AddRandomCircle();
    }

    if (GuiButton(Rectangle{ 60, 155, 110, 15 }, "Add 10 Rnd. Circles")) {
        for (int i = 0; i < 10; i++) {
            AddRandomCircle();
        }
    }

    if (GuiButton(Rectangle{ 60, 195, 110, 15 }, "Remove All Circles")) {
        while (AllCircles.size() != 0) {
            free(AllCircles.back());
            AllCircles.pop_back();
        }
    }

    DrawInfo(WIDTH - 315, HEIGHT - 5);

    GUIe = GuiSlider(Rectangle{ 164 , HEIGHT - 30, 95, 15 }, "Coefficient of Restitution  ", GUIeText.c_str(), GUIe, 0, 1);
    GUIeText = std::to_string(GUIe).substr(0, 3);

    DrawText("Press [SPACE] to pause.", WIDTH / 2 - MeasureText("Press [SPACE] to pause.", 10) / 2, 15, 10, GRAY);
    if (IsKeyPressed(KEY_SPACE)) {
        isPaused ^= 1;
    }
}

int main(void) {
	InitWindow(WIDTH, HEIGHT, TITLE);
	SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        if (!isPaused) {
            CheckCollisionsInVector(AllCircles);
            for (Circle* circle : AllCircles) {
                CheckBoundaryCollision(circle);
                MoveCircle(circle);
            }
        }

        BeginDrawing();
        DrawGUIBackground();
        DrawCircles();
        DrawBorders();
        DrawGUIForeground();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
	return 0;
}
