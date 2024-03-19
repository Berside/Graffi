#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <cstdlib> 
#include <ctime>
#include <chrono>
using namespace std;
// рандоманя генерация
struct Node {
    string data;
    Node* left;
    Node* right;
    Node() : data(""), left(nullptr), right(nullptr) {}
    Node(const string& data) : data(data), left(nullptr), right(nullptr) {}
};

Node* newNode(const string& data) {
    return new Node(data);
}
int findIndex(const std::string& str, int si, int ei) {
    stack<char> s;
    for (int i = si; i <= ei; i++) {
        if (str[i] == '(') {
            s.push(str[i]);
        }
        else if (str[i] == ')') {
            if (!s.empty() && s.top() == '(') {
                s.pop();
                if (s.empty()) {
                    return i;
                }
            }
        }
    }
    return -1;
}

int countRightLeafNodes(Node* node) {
    if (node == nullptr) return 0;
    int count = 0;
    if (node->right != nullptr && node->right->left == nullptr && node->right->right == nullptr) {
        count += 1;
        //cout << "Найден правый листовой узел: " << node->right->data << endl;
    }
    return count + countRightLeafNodes(node->left) + countRightLeafNodes(node->right);
}

Node* treeFromString(const std::string& str, int si, int ei) {
    if (si > ei) return nullptr;
    int num = 0;
    while (si <= ei && str[si] >= '0' && str[si] <= '9') {
        num = num * 10 + (str[si] - '0');
        si++;
    }
    Node* root = newNode(std::to_string(num));
    int index = -1;
    if (si <= ei && str[si] == '(') {
        index = findIndex(str, si, ei);
    }
    if (index != -1) {
        root->left = treeFromString(str, si + 1, index - 1);
        root->right = treeFromString(str, index + 2, ei - 1);
    }
    return root;
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
Node* readTreeFromFile(const std::string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Unable to open file " << filename << endl;
        return nullptr;
    }
    string str;
    getline(file, str);
    file.close();
    return treeFromString(str, 0, str.length() - 1);
}

void removeNodeAndDescendants(Node*& node) {
    if (node == nullptr) return;
    removeNodeAndDescendants(node->left);
    removeNodeAndDescendants(node->right);
    if (node->data == "0") {
        delete node;
        node = nullptr;
    }
}

void generateRandomBinaryTree(Node*& node, int n) {
    if (n == 0) return;

    // Randomly decide whether to add a left or right child
    int leftN = rand() % n;

    // Create a new node with a random value (for simplicity, we'll use "1" for all nodes)
    node = newNode("1");

    // Recursively generate the left subtree
    generateRandomBinaryTree(node->left, leftN);

    // Recursively generate the right subtree with the remaining nodes
    generateRandomBinaryTree(node->right, n - leftN - 1);
}

int main() {
    setlocale(LC_ALL, "Russian");
    Node* root = readTreeFromFile("Module.txt");
    if (root != nullptr) {
        removeNodeAndDescendants(root);
        printBT(root);
        cout << endl;
        cout << "Количество правых листовых узлов: " << countRightLeafNodes(root) << endl;

        srand(time(0));
        Node* root1 = nullptr;
        int n = 1000000;
        auto start = chrono::high_resolution_clock::now();
        generateRandomBinaryTree(root1, n);
        removeNodeAndDescendants(root1);
        //printBT(root1);

        cout << endl;
        cout << "Количество правых листовых узлов: " << countRightLeafNodes(root1) << endl;
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Execution time: " << duration << " milliseconds" << endl;
    }
    return 0;
}