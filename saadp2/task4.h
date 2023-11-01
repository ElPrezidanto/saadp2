#pragma once

#include <iostream>
#include <cstdlib>

#include <iostream>
#include <cstdlib>
#include <string>

enum Color { RED, BLACK };

struct Node {
    std::string data;
    Color color;
    Node* parent, * left, * right;

    Node(const std::string& val) : data(val), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}

};

class RedBlackTree {
private:
    Node* root;

    void leftRotate(Node* x);
    void rightRotate(Node* y);
    void insertFixup(Node* z);
    void insert(Node* root, Node* z);
    void inorderTraversal(Node* x);
    void preorderTraversal(Node* x);
    int findPathLength(Node* x, const std::string& value, int pathLength);
    int findHeight(Node* x, int height);

public:
    RedBlackTree() : root(nullptr) {}
    void insert(const std::string& value);
    void printInorder();
    int findPathLength(const std::string& value);
    int findHeight();
    void preorderTraversal();
    void displayMenu();
    void processChoice(int choice, RedBlackTree tree);
    void printTree(Node* node, std::string indent, bool last, bool isRoot = false) {
        if (node != nullptr) {
            std::cout << indent;
            if (last) {
                std::cout << (isRoot ? "Root----" : "R----");
                indent += "     ";
            }
            else {
                std::cout << "L----";
                indent += "|    ";
            }

            std::string sColor = (node->color == RED) ? "RED" : "BLACK";
            std::cout << node->data << " (" << sColor << ")" << std::endl;

            printTree(node->left, indent, false);
            printTree(node->right, indent, true);
        }
    }
    Node* getRoot() const {
        return root;
    }
};

void RedBlackTree::leftRotate(Node* x) {
    // ���������� ������ ��������
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RedBlackTree::rightRotate(Node* y) {
    // ���������� ������� ��������
    Node* x = y->left;
    y->left = x->right;
    if (x->right != nullptr) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == nullptr) {
        root = x;
    }
    else if (y == y->parent->left) {
        y->parent->left = x;
    }
    else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

void RedBlackTree::insertFixup(Node* z) {
    // ���� �����������, ���� �������� ������-������� ������ ��������
    while (z != nullptr && z->parent != nullptr && z->parent->parent != nullptr && z->parent->color == RED) {
        // �������� ������, ����� �������� z �������� ����� �������� ������ �������
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right; // y - ���� z
            // ������ 1: ���� ���� �������, �� ������������� ��������, ���� � �������, ����� ��������� ����� �� ������
            if (y != nullptr && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                // ������ 2: ���� ���� ������ � z - ������ �������, �� ��������� ����� �������� ������������ �������� z
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                // ������ 3: ���� ���� ������ � z - ����� �������, �� ������������� �������� � �������, ����� ��������� ������ �������� ������������ �������
                if (z->parent != nullptr) {
                    z->parent->color = BLACK;
                    if (z->parent->parent != nullptr) {
                        z->parent->parent->color = RED;
                        rightRotate(z->parent->parent);
                    }
                }
            }
        }
        else {
            // ����������� �������� ��� ������, ����� �������� z �������� ������ �������� ������ �������
            Node* y = z->parent->parent->left;
            if (y != nullptr && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK; // ������������� ����������� ����� � ������ ����
}



void RedBlackTree::insert(const std::string& value) {
    Node* z = new Node(value);
    if (root == nullptr) {
        root = z;
    }
    else {
        insert(root, z);
        insertFixup(z);
    }
}


void RedBlackTree::insert(Node* root, Node* z) {
    Node* y = nullptr;   // ��������� �� �������� ������ ����
    Node* x = root;      // ��������� ��� ����������� �� ������

    // ���� ����� ��� ������� ������ ���� � �������� ������ ������
    while (x != nullptr) {
        y = x;
        if (z->data < x->data) {
            x = x->left;    // ��������� �����, ���� �������� ������ ���� ������ ��������
        }
        else {
            x = x->right;   // ��������� ������, ���� �������� ������ ���� ������ ��� ����� ��������
        }
    }

    z->parent = y;          // ������������� �������� ��� ������ ����
    if (y == nullptr) {
        root = z;           // ���� ������ �����, ����� ���� ���������� ������
    }
    else if (z->data < y->data) {
        y->left = z;        // ���� �������� ������ ���� ������ �������������, ������ ��� �����
    }
    else {
        y->right = z;       // ���� �������� ������ ���� ������ ��� ����� �������������, ������ ��� ������
    }
}


void RedBlackTree::inorderTraversal(Node* x) {
    if (x != nullptr) {
        inorderTraversal(x->left);
        std::cout << x->data << " (" << (x->color == RED ? "Red" : "Black") << ") ";
        inorderTraversal(x->right);
    }
}

void RedBlackTree::preorderTraversal() {
    preorderTraversal(root);
    std::cout << std::endl;
}

void RedBlackTree::preorderTraversal(Node* x) {
    if (x != nullptr) {
        std::cout << x->data << " (" << (x->color == RED ? "Red" : "Black") << ") ";
        preorderTraversal(x->left);
        preorderTraversal(x->right);
    }
}
int RedBlackTree::findPathLength(Node* x, const std::string& value, int pathLength) {
    // ���������� ���������� ����� ���� �� ����� �� ���� � �������� ���������
    if (x == nullptr) {
        return 0;
    }

    if (value < x->data) {
        return findPathLength(x->left, value, pathLength + 1);
    }
    else if (value > x->data) {
        return findPathLength(x->right, value, pathLength + 1);
    }
    else {
        return pathLength;
    }
}

int RedBlackTree::findHeight(Node* x, int height) {
    // ���������� ���������� ������ ������
    if (x == nullptr) {
        return height - 1;
    }

    int leftHeight = findHeight(x->left, height + 1);
    int rightHeight = findHeight(x->right, height + 1);

    return std::max(leftHeight, rightHeight);
}

void RedBlackTree::printInorder() {
    inorderTraversal(root);
    std::cout << std::endl;
}

int RedBlackTree::findPathLength(const std::string& value) {
    return findPathLength(root, value, 0);
}

int RedBlackTree::findHeight() {
    return findHeight(root, 0);
}

//void RedBlackTree::printTree(Node* node, string indent, bool last, bool isRoot = false) {
//    if (node != nullptr) {
//        cout << indent;
//        if (last) {
//            cout << (isRoot ? "Root----" : "R----");
//            indent += "     ";
//        }
//        else {
//            cout << "L----";
//            indent += "|    ";
//        }
//
//        string sColor = (node->color == RED) ? "RED" : "BLACK";
//        cout << node->data << " (" << sColor << ")" << endl;
//
//        printTree(node->left, indent, false);
//        printTree(node->right, indent, true);
//    }
//}

void RedBlackTree::displayMenu() {
    std::cout << "Menu:\n";
    std::cout << "1. Insert Element\n";
    std::cout << "2. Display Inorder Traversal\n";
    std::cout << "3. Display Preorder Traversal\n";
    std::cout << "4. Find Path Length to Value\n";
    std::cout << "5. Find Tree Height\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter your choice: ";
}

void RedBlackTree::processChoice(int choice, RedBlackTree tree) {
    std::string value;
    switch (choice) {
    case 1:
        std::cout << "Enter value to insert: ";
        std::cin >> value;
        insert(value);
        break;
    case 2:
        std::cout << "Inorder Traversal: ";
        printInorder();
        break;
    case 3:
        std::cout << "Preorder Traversal: ";
        preorderTraversal();
        break;
    case 4:
        std::cout << "Enter value to find path length: ";
        std::cin >> value;
        std::cout << "Path length to " << value << ": " << findPathLength(value) << std::endl;
        break;
    case 5:
        std::cout << "Tree Height: " << findHeight() << std::endl;
        break;
    case 6:
        std::cout << "Print the tree:" << std::endl;
        tree.printTree(tree.getRoot(), "", true, true);
        break;
    case 7:
        std::cout << "Exiting program.\n";
        exit(0);
    default:
        std::cout << "Invalid choice.\n";
    }
}

void num4() {
    RedBlackTree tree;
    int choice;

    while (true) {
        tree.displayMenu();
        std::cin >> choice;
        tree.processChoice(choice, tree);
     }
}
