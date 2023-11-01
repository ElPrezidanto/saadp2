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
    // Реализация левого поворота
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
    // Реализация правого поворота
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
    // Цикл выполняется, пока свойства красно-черного дерева нарушены
    while (z != nullptr && z->parent != nullptr && z->parent->parent != nullptr && z->parent->color == RED) {
        // Проверка случая, когда родитель z является левым потомком своего дедушки
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right; // y - дядя z
            // Случай 1: Если дядя красный, то перекрашиваем родителя, дядю и дедушку, затем двигаемся вверх по дереву
            if (y != nullptr && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                // Случай 2: Если дядя чёрный и z - правый потомок, то выполняем левое вращение относительно родителя z
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                // Случай 3: Если дядя чёрный и z - левый потомок, то перекрашиваем родителя и дедушку, затем выполняем правое вращение относительно дедушки
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
            // Аналогичные действия для случая, когда родитель z является правым потомком своего дедушки
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
    root->color = BLACK; // Окончательное окрашивание корня в чёрный цвет
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
    Node* y = nullptr;   // Указатель на родителя нового узла
    Node* x = root;      // Указатель для перемещения по дереву

    // Ищем место для вставки нового узла в бинарное дерево поиска
    while (x != nullptr) {
        y = x;
        if (z->data < x->data) {
            x = x->left;    // Двигаемся влево, если значение нового узла меньше текущего
        }
        else {
            x = x->right;   // Двигаемся вправо, если значение нового узла больше или равно текущему
        }
    }

    z->parent = y;          // Устанавливаем родителя для нового узла
    if (y == nullptr) {
        root = z;           // Если дерево пусто, новый узел становится корнем
    }
    else if (z->data < y->data) {
        y->left = z;        // Если значение нового узла меньше родительского, ставим его слева
    }
    else {
        y->right = z;       // Если значение нового узла больше или равно родительскому, ставим его справа
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
    // Реализация нахождения длины пути от корня до узла с заданным значением
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
    // Реализация нахождения высоты дерева
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
