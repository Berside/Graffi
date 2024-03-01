#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
using namespace std;


struct Node {
	int data;
	Node* left;
	Node* rignt;
};

Node* newNode(int data) {
	Node* newNode = new Node();
	newNode->data = data;
	newNode->left = newNode->rignt = NULL;
	return newNode;
}

int RightCount(Node* root) {
	if (root == nullptr) return 0;
	vector <Node*> v;
	int count = 0;

	while (!v.empty()) {
		Node* node = v.front();
		v.pop_back();
		if ((node->left == nullptr) and (node->rignt == nullptr)) {
			Node* parent = nullptr;
			while (!v.empty()) {
				parent = v.front();
				v.pop_back();
				if (parent->left == node) break;
				if ((parent == nullptr) and (parent->rignt == node)) {
					count++;
				}
			}
		}
		if (node->left != nullptr) v.push_back(node->left);
		if (node->rignt != nullptr) v.push_back(node->rignt);
	}
	return count;
}

int main() {
	// ifstream fs("strings.txt", ios::in | ios::binary);
	// fs.close();
	Node* root = newNode(1);
	root->left = newNode(2);
	root->rignt = newNode(3);
	root->left->left = newNode(4);
	root->left->rignt = newNode(5);
	root->rignt->left = newNode(6);
	root->rignt->rignt = newNode(7);
	cout << "Count right leaves:" << RightCount(root) << endl;
	return 0;
} 