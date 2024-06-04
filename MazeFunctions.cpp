#include "MazeFunctions.h"
#include "Node.h"
#include "Chaser.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>

using namespace std;
using namespace sf;

#define N 20
#define M 20
#define CHASER_START_DELAY 100

void linkNodes(vector<Node*>& nodes) {
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            Node* current = nodes[i * M + j];
            if (i > 0) current->up = nodes[(i - 1) * M + j];
            if (j > 0) current->left = nodes[i * M + (j - 1)];
            if (i < N - 1) current->down = nodes[(i + 1) * M + j];
            if (j < M - 1) current->right = nodes[i * M + (j + 1)];
        }
    }
}

Node* createRandomMaze(vector<Node*>& nodes) {
    nodes.clear();
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            bool isWall = rand() % 2 == 0;
            Node* newNode = new Node(i, j, isWall);
            nodes.push_back(newNode);
        }
    }

    linkNodes(nodes);

    nodes[0]->isWall = false;
    nodes[N * M - 1]->isWall = false;

    for (int i = 0; i < N * M; ++i)
    {
        if (nodes[i]->isWall) continue;
        int nonWallNeighbors = 0;
        if (nodes[i]->up && !nodes[i]->up->isWall) nonWallNeighbors++;
        if (nodes[i]->down && !nodes[i]->down->isWall) nonWallNeighbors++;
        if (nodes[i]->left && !nodes[i]->left->isWall) nonWallNeighbors++;
        if (nodes[i]->right && !nodes[i]->right->isWall) nonWallNeighbors++;

        while (nonWallNeighbors < 2)
        {
            int randDir = rand() % 4;
            if (randDir == 0 && nodes[i]->up && nodes[i]->up->isWall)
            {
                nodes[i]->up->isWall = false;
                nonWallNeighbors++;
            }
            else if (randDir == 1 && nodes[i]->down && nodes[i]->down->isWall)
            {
                nodes[i]->down->isWall = false;
                nonWallNeighbors++;
            }
            else if (randDir == 2 && nodes[i]->left && nodes[i]->left->isWall)
            {
                nodes[i]->left->isWall = false;
                nonWallNeighbors++;
            }
            else if (randDir == 3 && nodes[i]->right && nodes[i]->right->isWall)
            {
                nodes[i]->right->isWall = false;
                nonWallNeighbors++;
            }
        }
    }

    return nodes[0];
}

bool hasReachedExit(Node* player, Node* exitNode) {
    return (player->x == exitNode->x && player->y == exitNode->y);
}

void findPath(Node* start, Node* goal, Chaser& chaser, const vector<Node*>& allNodes) {
    for (auto& node : allNodes)
    {
        node->parent = nullptr;
    }
    queue<Node*> q;
    q.push(start);
    start->parent = start;

    while (!q.empty())
    {
        Node* current = q.front();
        q.pop();

        // Goal check
        if (current == goal)
        {
            while (current->parent != start)
            {
                current = current->parent;
            }
            chaser.target = current;
            return;
        }

        if (current->up && !current->up->isWall && current->up->parent == nullptr)
        {
            current->up->parent = current;
            q.push(current->up);
        }
        if (current->down && !current->down->isWall && current->down->parent == nullptr)
        {
            current->down->parent = current;
            q.push(current->down);
        }
        if (current->left && !current->left->isWall and current->left->parent == nullptr)
        {
            current->left->parent = current;
            q.push(current->left);
        }
        if (current->right && !current->right->isWall && current->right->parent == nullptr)
        {
            current->right->parent = current;
            q.push(current->right);
        }
    }
    chaser.target = nullptr;
}

void displayMaze(RenderWindow& window, vector<Node*>& nodes, Node* player, Node* exitNode, Chaser& chaser, Sprite& pacmanSprite) {
    float cellSize = 40.0f;
    RectangleShape wall(Vector2f(cellSize - 20.0f, cellSize - 20.0f));
    wall.setFillColor(Color(50, 50, 50));
    RectangleShape path(Vector2f(cellSize, cellSize));
    path.setFillColor(Color(211, 211, 211));
    RectangleShape exitShape(Vector2f(cellSize, cellSize));
    exitShape.setFillColor(Color::Green);

    window.clear(Color(30, 30, 30));
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            Node* node = nodes[i * M + j];
            Vector2f position(j * cellSize, i * cellSize);
            if (node->isWall)
            {
                wall.setPosition(position);
                window.draw(wall);
            }
            else
            {
                path.setPosition(position);
                window.draw(path);
            }
        }
    }
    exitShape.setPosition(exitNode->y * cellSize, exitNode->x * cellSize);
    window.draw(exitShape);
    pacmanSprite.setPosition(player->y * cellSize, player->x * cellSize);
    window.draw(pacmanSprite);
    if (chaser.delay <= 0)
    {
        window.draw(chaser.sprite);
    }

    window.display();
}

void showWelcomeScreen(RenderWindow& window) {
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error loading font\n";
    }

    Text title("Maze Pursuit", font, 50);
    title.setFillColor(Color::White);
    FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    title.setPosition(Vector2f(window.getSize().x / 2.0f, 150));

    Text play("Press P to Play", font, 30);
    play.setFillColor(Color::Green);
    FloatRect playRect = play.getLocalBounds();
    play.setOrigin(playRect.left + playRect.width / 2.0f, playRect.top + playRect.height / 2.0f);
    play.setPosition(Vector2f(window.getSize().x / 2.0f, 300));

    Text quit("Press Q to Quit", font, 30);
    quit.setFillColor(Color::Red);
    FloatRect quitRect = quit.getLocalBounds();
    quit.setOrigin(quitRect.left + quitRect.width / 2.0f, quitRect.top + quitRect.height / 2.0f);
    quit.setPosition(Vector2f(window.getSize().x / 2.0f, 350));

    window.clear();
    window.draw(title);
    window.draw(play);
    window.draw(quit);
    window.display();
}

void showEndScreen(RenderWindow& window, int score, bool& playAgain) {
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error loading font\n";
    }

    Text title("Game Over!", font, 50);
    title.setFillColor(Color::White);
    FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    title.setPosition(Vector2f(window.getSize().x / 2.0f, 150));

    Text scoreDisplay("Your Score: " + to_string(score), font, 30);
    scoreDisplay.setFillColor(Color::Yellow);
    FloatRect scoreRect = scoreDisplay.getLocalBounds();
    scoreDisplay.setOrigin(scoreRect.left + scoreRect.width / 2.0f, scoreRect.top + scoreRect.height / 2.0f);
    scoreDisplay.setPosition(Vector2f(window.getSize().x / 2.0f, 300));

    Text replay("Press R to Play Again", font, 30);
    replay.setFillColor(Color::Green);
    FloatRect replayRect = replay.getLocalBounds();
    replay.setOrigin(replayRect.left + replayRect.width / 2.0f, replayRect.top + replayRect.height / 2.0f);
    replay.setPosition(Vector2f(window.getSize().x / 2.0f, 350));

    Text quit("Press Q to Quit", font, 30);
    quit.setFillColor(Color::Red);
    FloatRect quitRect = quit.getLocalBounds();
    quit.setOrigin(quitRect.left + quitRect.width / 2.0f, quitRect.top + quitRect.height / 2.0f);
    quit.setPosition(Vector2f(window.getSize().x / 2.0f, 400));

    window.clear();
    window.draw(title);
    window.draw(scoreDisplay);
    window.draw(replay);
    window.draw(quit);
    window.display();

    Event event;
    while (true) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                playAgain = false;
                return;
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::R) {
                    playAgain = true;
                    return;
                }
                else if (event.key.code == Keyboard::Q) {
                    playAgain = false;
                    return;
                }
            }
        }
    }
}
