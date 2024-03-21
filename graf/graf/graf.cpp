#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <ctime>
#include <memory>
#include <chrono>
using namespace std;

// Структура Node представляет собой узел в бинарном дереве, где каждый узел содержит данные типа string и указатели на левого и правого потомка, которые также являются узлами.
struct Node {
    string data;
    unique_ptr<Node> left;
    unique_ptr<Node> right;
    Node() : data(""), left(nullptr), right(nullptr) {}
    Node(const string& data) : data(data), left(nullptr), right(nullptr) {}
};

// Создает новый узел с заданными данными.
unique_ptr<Node> newNode(const string& data) {
    return std::make_unique<Node>(data);
}

// Находит индекс закрывающей скобки, соответствующей открывающей скобке, начиная с позиции si.
int findIndex(const string& str, int si, int ei) {
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

// Подсчитывает количество правых листовых узлов в дереве.
int countRightLeafNodes(unique_ptr<Node>& node, int k) {
    if (!node) return 0;
    int count = 0;
    if (node->right && !node->right->left && !node->right->right) {
        count += 1;
        if (k == 1) {
            cout << "Найден правый листовой узел: " << node->right->data << endl;
        }
    }
    return count + countRightLeafNodes(node->left, k) + countRightLeafNodes(node->right, k);
}

// Создает дерево из строки, представляющей его структуру.
unique_ptr<Node> treeFromString(const string& str, int si, int ei) {
    if (si > ei) return nullptr;
    int num = 0;
    while (si <= ei && str[si] >= '0' && str[si] <= '9') {
        num = num * 10 + (str[si] - '0');
        si++;
    }
    unique_ptr<Node> root = newNode(to_string(num));
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

// Выводит дерево в виде текстового представления.
void printBT(const string& prefix, unique_ptr<Node>& node, bool isLeft) {
    if (node) {
        cout << prefix << (isLeft ? "|--" : "|__") << node->data << endl;
        printBT(prefix + (isLeft ? "|   " : "    "), node->right, true);
        printBT(prefix + (isLeft ? "|   " : "    "), node->left, false);
    }
}


// Читает дерево из файла, представленного строкой.
unique_ptr<Node> readTreeFromFile(const std::string& filename) {
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

// Удаляет узел и все его потомки.
void removeNodeAndDescendants(unique_ptr<Node>& node) {
    if (!node) return;
    removeNodeAndDescendants(node->left);
    removeNodeAndDescendants(node->right);
    if (node->data == "0") {
        node.reset();
    }
}

// Генерирует случайное бинарное дерево с заданным количеством узлов.
void generateRandomBinaryTree(unique_ptr<Node>& node, int n) {
    if (n == 0) return;
    int leftN = rand() % n;
    node = newNode("1");
    generateRandomBinaryTree(node->left, leftN);
    generateRandomBinaryTree(node->right, n - leftN - 1);
}

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "Выберите способ запуск программы:" << endl;
    cout << "Если вы хотите построить дерево используя входные данные. Напишите: 1" << endl;
    cout << "Если вы хотите построить дерево используя случайную генерацию. Напишите: 2" << endl;
    cout << "Если вы не хотите строить бинарное дерево. Напишите любое число" << endl;
    cout << endl;
    int k;
    cin >> k;
    if (k == 1) {
        auto start1 = chrono::high_resolution_clock::now();
        unique_ptr<Node> root = readTreeFromFile("Module.txt");
        if (root) {
            removeNodeAndDescendants(root);
            printBT("", root, false);
            cout << endl;
            auto start2 = chrono::high_resolution_clock::now();
            cout << "Количество правых листовых узлов: " << countRightLeafNodes(root, k) << endl;
            auto end2 = chrono::high_resolution_clock::now();
            auto end1 = chrono::high_resolution_clock::now();
            auto duration1 = chrono::duration_cast<chrono::milliseconds>(end1 - start1).count();
            auto duration2 = chrono::duration_cast<chrono::milliseconds>(end2 - start2).count();
            cout << "Время выполения алгоритма: " << duration2 << " milliseconds" << endl;
            cout << "Время выполения программы: " << duration1 << " milliseconds" << endl;
        }
    }
    if (k == 2) {
        srand(time(0));
        unique_ptr<Node> root1 = nullptr;
        int n;
        cout << "Напиши количество узлов для генерации бинарного дерева:" << endl;
        cin >> n;
        auto start = chrono::high_resolution_clock::now();
        generateRandomBinaryTree(root1, n);
        cout << endl;
        auto start2 = chrono::high_resolution_clock::now();
        cout << "Количество правых листовых узлов: " << countRightLeafNodes(root1, k) << endl;
        auto end2 = chrono::high_resolution_clock::now();
        auto end = chrono::high_resolution_clock::now();
        auto duration2 = chrono::duration_cast<chrono::milliseconds>(end2 - start2).count();
        cout << "Время выполения алгоритма: " << duration2 << " milliseconds" << endl;
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "Время выполения программы: " << duration << " milliseconds" << endl;
    }
    return 0;
}
