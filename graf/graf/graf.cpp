#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <ctime>
using namespace std;

struct Node {
    char data;
    Node* left;
    Node* right;
    Node() : data('\0'), left(nullptr), right(nullptr) {}
    Node(char data) : data(data), left(nullptr), right(nullptr) {}
};

Node* newNode(char data) {
    return new Node(data);
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

void printBT(const string& prefix, Node* node, bool isLeft) {
    if (node != nullptr) {
        cout << prefix << (isLeft ? "|--" : "|__") << node->data << endl;
        printBT(prefix + (isLeft ? "|   " : "    "), node->right, true);
        printBT(prefix + (isLeft ? "|   " : "    "), node->left, false);
    }
}

void printBT(Node* node) {
    printBT("", node, false);
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
    while (file >> parent >> separator >> child) {
        if (separator != '-' && separator != '>') {
            cerr << "Неверный формат данных в файле." << endl;
            return nullptr;
        }
        if (nodeMap.find(parent) == nodeMap.end()) {
            nodeMap[parent] = newNode(parent);
        }
        if (nodeMap.find(child) == nodeMap.end()) {
            nodeMap[child] = newNode(child);
        }
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
        if (root == nullptr) {
            root = nodeMap[parent];
        }
    }
    return root;
}
void writeTreeToFile(Node* node, std::ofstream& file, std::string parent = "") {
   if (node == nullptr) return;

   std::string child = std::string(1, node->data);
   if (!parent.empty()) {
       file << parent << "-" << child << "\n";
   }

   writeTreeToFile(node->left, file, child);
   writeTreeToFile(node->right, file, child);
}

Node* generateRandomBinaryTree(int n) {
    if (n == 0) return nullptr;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(48, 122);
    char data = static_cast<char>(dis(gen));
    Node* root = newNode(data);
    int leftN = rand() % n;
    int rightN = n - leftN - 1;
    root->left = generateRandomBinaryTree(leftN);
    root->right = generateRandomBinaryTree(rightN);

    // Write the tree to a file
    std::ofstream file("binary_tree.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return nullptr;
    }
    writeTreeToFile(root, file);
    file.close();

    return root;
}

int main() {
    setlocale(LC_ALL, "Russian");
    auto start = chrono::high_resolution_clock::now();
    Node* root2 = createTreeFromFile("Module.txt");
    int count2 = countRightLeafNodes(root2);
    cout << "Count Right Leaf Nodes: " << count2 << endl << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Execution time: " << duration << " milliseconds" << endl;
    printBT(root2);
    cout << endl;

    srand(time(0));
    int n = 50;
    Node* root = generateRandomBinaryTree(n);
    int count = countRightLeafNodes(root);
    cout << "Count Right Leaf Nodes: " << count << endl << endl;
    printBT(root);
}

