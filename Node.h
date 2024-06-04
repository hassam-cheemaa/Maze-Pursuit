#ifndef NODE_H
#define NODE_H

class Node {
public:
    int x, y;
    bool isWall;
    Node* up, * down, * left, * right;
    Node* parent; // head

    Node(int x, int y, bool wall);
};

#endif
