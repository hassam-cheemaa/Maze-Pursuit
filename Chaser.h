#ifndef CHASER_H
#define CHASER_H

#include <SFML/Graphics.hpp>

class Node;

class Chaser {
public:
    float x, y;
    float speed;
    Node* target;
    int delay;
    sf::Texture texture;
    sf::Sprite sprite;

    Chaser(float x, float y, float speed);
    void update();
    bool hasCaughtPlayer(float playerX, float playerY);
};

#endif
