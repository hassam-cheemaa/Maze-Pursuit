#ifndef MAZEFUNCTIONS_H
#define MAZEFUNCTIONS_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Chaser.h"

void linkNodes(std::vector<Node*>& nodes);
Node* createRandomMaze(std::vector<Node*>& nodes);
bool hasReachedExit(Node* player, Node* exitNode);
void findPath(Node* start, Node* goal, Chaser& chaser, const std::vector<Node*>& allNodes);
void displayMaze(sf::RenderWindow& window, std::vector<Node*>& nodes, Node* player, Node* exitNode, Chaser& chaser, sf::Sprite& pacmanSprite);
void showWelcomeScreen(sf::RenderWindow& window);
void showEndScreen(sf::RenderWindow& window, int score, bool& playAgain);

#endif // MAZEFUNCTIONS_H
