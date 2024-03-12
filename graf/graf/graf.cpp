#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <ctime> // Добавьте эту библиотеку
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
};

Node* newNode(int data, Node* parent = nullptr) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->left = newNode->right = nullptr;
    newNode->parent = parent;
    return newNode;
}

int RightCount(Node* root) {
    if (root == nullptr) return 0;
    int count = 0;
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        // Проверяем, является ли текущий узел листом и правым дочерним вершином
        if (current->left == nullptr && current->right == nullptr && current->parent && current->parent->right == current) {
            count++;
        }
            
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    return count;
}


Node* readTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        return nullptr;
    }
    queue<Node*> nodeQueue;
    Node* root = nullptr;
    string line;
    int lineCount = 0;
    while (getline(file, line)) {
        lineCount++;
        int data;
        if (!(file >> data)) break;
        Node* node = newNode(data);

        if (lineCount == 1) {
            root = node;
            nodeQueue.push(root);
        }
        else {
            if (nodeQueue.empty()) {
                cerr << "Unexpected EOF" << endl;
                return nullptr;
            }
            Node* parent = nodeQueue.front();
            nodeQueue.pop();

            int leftChildData, rightChildData;
            file >> leftChildData >> rightChildData;

            if (leftChildData != -1) {
                node->left = newNode(leftChildData, node);
                nodeQueue.push(node->left);
            }
            if (rightChildData != -1) {
                node->right = newNode(rightChildData, node);
                nodeQueue.push(node->right);
            }

            if (parent->left == nullptr) {
                parent->left = node;
            }
            else if (parent->right == nullptr) {
                parent->right = node;
            }
        }
    }
    return root;
}


void printTree(Node* node, int space = 0) {
    if (node == nullptr) return;
    printTree(node->right, space + 10);
    cout << endl;
    for (int i = 10; i < space; i++) {
        cout << " ";
    }
    cout << node->data << "\n";
    printTree(node->left, space + 10);
}

int main() {
    setlocale(LC_ALL, "Russian");

    // Запоминаем время начала выполнения
    clock_t start = clock();

    Node* root = readTreeFromFile("Module.txt");
    if (root == nullptr) {
        cerr << "Не удалось прочитать дерево из файла." << endl;
        return 1;
    }
    cout << "Бинарное дерево: " << endl;
    printTree(root);
    cout << "Count right leaves: " << RightCount(root) << endl;

    // Вычисляем и выводим время выполнения
    clock_t end = clock();
    double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "Время выполнения: " << duration << " секунд" << endl;

    return 0;
}

