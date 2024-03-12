#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
using namespace std;
// Функция createTreeFromFile учитывает только название с одной буквой 
struct Node {
    char data;
    Node* left;
    Node* right;
    Node() : data('\0'), left(nullptr), right(nullptr) {}
    Node(char data) : data(data), left(nullptr), right(nullptr) {}
};

Node* newNode(char data) {
    Node* node = new Node;
    node->data = data;
    node->left = node->right = nullptr;
    return node;
}

int countRightLeafNodes(Node* node) {
    if (node == nullptr) return 0;
    int count = 0;
    if (node->right != nullptr and node->right->left == nullptr and node->right->right == nullptr) {
        count += 1;
        cout << "Найден правый листовой узел: " << node->right->data << endl;
    }
    return count + countRightLeafNodes(node->left) + countRightLeafNodes(node->right);
}

void takeInput(Node*& node) {
    char data;
    cout << "Enter data for the node: ";
    cin >> data;
    node = newNode(data);
    cout << "Do you want to add a left child? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        takeInput(node->left);
    }
    cout << "Do you want to add a right child? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        takeInput(node->right);
    }
}

Node* InputBinaryTree() {
    Node* root = nullptr;
    takeInput(root);
    return root;
}

void printTree(Node* node, int space) {
    if (node == nullptr)
        return;
    space += 10;
    printTree(node->right, space);
    cout << endl;
    for (int i = 10; i < space; i++)
        cout << " ";
    cout << node->data << "\n";
    printTree(node->left, space);
}

Node* findNode(Node* root, char data) {
    if (!root) return nullptr;
    if (root->data == data) return root;
    Node* left = findNode(root->left, data);
    if (left) return left;
    return findNode(root->right, data);
}

Node* createTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл " << filename << endl;
        return nullptr;
    }

    map<char, Node*> nodeMap;
    char parent, child, separator;
    Node* root = nullptr;
    bool isFirstLine = true;
    while (file >> parent >> separator >> child) {
        if (separator != '-' && separator != '>') {
            cerr << "Неверный формат данных в файле." << endl;
            return nullptr;
        }

        // Добавляем узлы в nodeMap сразу после их создания
        if (nodeMap.find(parent) == nodeMap.end()) {
            nodeMap[parent] = newNode(parent);
        }
        if (nodeMap.find(child) == nodeMap.end()) {
            nodeMap[child] = newNode(child);
        }

        // Связываем дочерний узел с родительским
        if (separator == '-') {
            if (nodeMap[parent]->left == nullptr) {
                nodeMap[parent]->left = nodeMap[child];
            }
            else if (nodeMap[parent]->right == nullptr) {
                nodeMap[parent]->right = nodeMap[child];
            }
            else {
                cerr << "Ошибка: узел с данными " << parent << " имеет более двух потомков." << endl;
                return nullptr;
            }
        }
        else if (separator == '>') {
            if (nodeMap[parent]->right == nullptr) {
                nodeMap[parent]->right = nodeMap[child];
            }
            else {
                cerr << "Ошибка: узел с данными " << parent << " уже имеет правого потомка." << endl;
                return nullptr;
            }
        }

        // Если это первая строка файла, устанавливаем корневой узел
        if (isFirstLine) {
            root = nodeMap[parent];
            isFirstLine = false;
        }
    }

    return root;
}


int main() {
    setlocale(LC_ALL, "Russian");
    auto start = chrono::high_resolution_clock::now();
    Node* root2 = createTreeFromFile("Module.txt");
    int count2 = countRightLeafNodes(root2);
    cout << "Count Right Leaf Nodes: " << count2 << endl << endl;
    printTree(root2, 0);
    Node* root = newNode('A');
    root->left = newNode('B');
    root->right = newNode('C');
    root->left->left = newNode('D');
    root->left->right = newNode('E');
    root->right->right = newNode('F');
    root->left->left->left = newNode('G');
    root->left->left->right = newNode('H');
    int count = countRightLeafNodes(root);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    //cout << "Count Right Leaf Nodes: " << count << endl;
    cout << "Execution time: " << duration << " microseconds" << endl;
    //printTree(root, 0);
}

