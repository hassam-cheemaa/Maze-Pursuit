#include "Node.h"

Node::Node(int x, int y, bool wall) : x(x), y(y), isWall(wall), up(nullptr), down(nullptr), left(nullptr), right(nullptr), parent(nullptr) {}
