#include <SFML/Graphics.hpp>
#include<iostream>
#include <vector>
#include <ctime>
#include "MazeFunctions.h"

using namespace std;
using namespace sf;

#define N 20
#define M 20
#define CHASER_START_DELAY 100 


int main()
{
    RenderWindow window(VideoMode(M * 40, N * 40), "Maze Pursuit");
    window.setFramerateLimit(60);
    srand(time(0));

    bool playAgain = true;
    while (playAgain) {
        // Welcome screen
        showWelcomeScreen(window);
        bool startGame = false;
        Event event;
        while (window.isOpen() && !startGame) {
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::P) {
                        startGame = true;
                    }
                    else if (event.key.code == Keyboard::Q) {
                        window.close();
                        return 0;
                    }
                }
            }
        }

        vector<Node*> allNodes;
        Node* start = createRandomMaze(allNodes);
        Node* current = start;
        Node* exitNode = allNodes[N * M - 1];

        Chaser chaser(0, 0, 3.0f);
        bool chaserActive = false;
        float chaserActivationDelay = 3.0f;
        Clock clock;
        Clock gameClock;

        Texture pacmanTexture;
        if (!pacmanTexture.loadFromFile("pacman.png")) {
            cerr << "Error loading pacman texture" << endl;
            return -1;
        }
        Sprite pacmanSprite(pacmanTexture);
        pacmanSprite.setScale(0.15f, 0.15f);

        int score = 0;
        while (window.isOpen())
        {
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }

                if (event.type == Event::KeyPressed)
                {
                    if (event.key.code == Keyboard::Up && current->up && !current->up->isWall)
                    {
                        current = current->up;
                    }
                    else if (event.key.code == Keyboard::Down && current->down && !current->down->isWall)
                    {
                        current = current->down;
                    }
                    else if (event.key.code == Keyboard::Left and current->left && !current->left->isWall)
                    {
                        current = current->left;
                    }
                    else if (event.key.code == Keyboard::Right && current->right && !current->right->isWall)
                    {
                        current = current->right;
                    }
                    else if (event.key.code == Keyboard::Escape)
                    {
                        window.close();
                    }
                }
            }
            if (!chaserActive and clock.getElapsedTime().asSeconds() > chaserActivationDelay)
            {
                chaserActive = true;
            }
            if (chaserActive)
            {
                findPath(allNodes[static_cast<int>(chaser.x / 40.0f) * M + static_cast<int>(chaser.y / 40.0f)], current, chaser, allNodes);

                chaser.update();
            }
            displayMaze(window, allNodes, current, exitNode, chaser, pacmanSprite);
            if (hasReachedExit(current, exitNode))
            {
                cout << "You reached the exit! Generating new maze..." << endl;
                score += 1000 - static_cast<int>(gameClock.getElapsedTime().asSeconds());
                start = createRandomMaze(allNodes);
                current = start;
                exitNode = allNodes[N * M - 1];
                chaserActive = false;
                clock.restart();
                gameClock.restart();
            }

            if (chaserActive and chaser.hasCaughtPlayer(current->x * 40, current->y * 40))
            {
                cout << "Game Over! Chaser caught the player!" << endl;
                break;
            }
        }

        showEndScreen(window, score, playAgain);
    }

    return 0;
}
