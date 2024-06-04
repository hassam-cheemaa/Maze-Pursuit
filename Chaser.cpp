#include "Chaser.h"
#include "Node.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#define CHASER_START_DELAY 100

Chaser::Chaser(float x, float y, float speed) : x(x), y(y), speed(speed), target(nullptr), delay(CHASER_START_DELAY) {
    if (!texture.loadFromFile("ghost.png")) {
        std::cerr << "Error loading ghost texture" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.08f, 0.08f);
}

void Chaser::update() {
    if (delay > 0)
    {
        --delay;
        return;
    }
    if (target)
    {
        float targetX = target->x * 40.0f;
        float targetY = target->y * 40.0f;

        float dx = targetX - x;
        float dy = targetY - y;
        float magnitude = sqrt(dx * dx + dy * dy);
        if (magnitude < speed)
        {
            x = targetX;
            y = targetY;
        }
        else
        {
            dx /= magnitude;
            dy /= magnitude;
            x += dx * speed;
            y += dy * speed;
        }

        target = target->parent;
    }
    sprite.setPosition(y, x);
}

bool Chaser::hasCaughtPlayer(float playerX, float playerY) {
    float distance = sqrt((x - playerX) * (x - playerX) + (y - playerY) * (y - playerY));
    return distance < 0.5;
}
