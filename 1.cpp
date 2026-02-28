#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

class AVLTree {
    struct Node {
        Node* left = nullptr;
        Node* right = nullptr;
        int value;
        int height = 1;

        Node(int value): value(value) {}

        Node(const Node& other): value(other.value), height(other.height) {
            if (other.left) left = new Node{*other.left};
            if (other.right) right = new Node{*other.right};
        }
    };

    Node* root = nullptr;

    Node* _insert(Node* node, int value) {
        if (!node) return new Node{value};
        if (value > node->value) node->right = _insert(node->right, value);
        else node->left = _insert(node->left, value);
        return balance(node);
    }

    Node* _find(Node* node, int value) {
        if (!node) return nullptr;
        if (node->value == value) return node;
        if (value > node->value) return _find(node->right, value);
        return _find(node->left, value);
    }

    Node* _remove(Node* node, int value) {
        if (!node) return nullptr;
        if (value < node->value) node->left = _remove(node->left, value);
        else if (value > node->value) node->right = _remove(node->right, value);
        else {
            if (!node->left and !node->right) {
                delete node;
                node = nullptr;
                return node;
            }
            else if (!node->left or !node->right) {
                Node* res;
                if (node->left) res = node->left;
                else res = node->right;
                delete node;
                node = nullptr;
                return res;
            }
            else {
                int min = _min(node->right);
                node->value = min;
                node->right = _remove(node->right, min);
            }
        }
        return balance(node);
    }

    int _min(Node* node) {
        if (!node->left) return node->value;
        return _min(node->left);
    }

    int _max(Node* node) {
        if (!node->right) return node->value;
        return _max(node->right);
    }

    Node* balance(Node* node) {
        calc_height(node);
        if (get_balance(node) == 2) {
            if (get_balance(node->right) == -1)
                node->right = right_rotate(node->right);
            return left_rotate(node);
        }

        if (get_balance(node) == -2) {
            if (get_balance(node->left) == 1)
                node->left = left_rotate(node->left);
            return right_rotate(node);
        }
        return node;
    }

    Node* right_rotate(Node* node) {
        Node* left = node->left;
        node->left = left->right;
        left->right = node;
        calc_height(node);
        calc_height(left);
        return left;
    }

    Node* left_rotate(Node* node) {
        Node* right = node->right;
        node->right = right->left;
        right->left = node;
        calc_height(node);
        calc_height(right);
        return right;
    }

    int get_balance(Node* node) {
        return height(node->right) - height(node->left);
    }

    void calc_height(Node* node) {
        int left_height = height(node->left);
        int right_height = height(node->right);
        node->height = std::max(left_height, right_height) + 1;
    }

    int height(Node* node) {
        if (!node) return 0;
        return node->height;
    }

    void _inorder(Node* node, vector<int>& result) {
        if (!node) return;
        _inorder(node->left, result);
        result.push_back(node->value);
        _inorder(node->right, result);
    }

    void _delete(Node* node) {
        if (!node) return;
        _delete(node->left);
        _delete(node->right);
        delete node;
    }

public:
    AVLTree() = default;

    AVLTree(const AVLTree &other) {
        if (other.root) root = new Node(*other.root);
    }

    ~AVLTree() {
        clear();
    }

    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            clear();
            if (other.root) root = new Node(*other.root);
        }
        return *this;
    }

    void insert(int value) {
        root = _insert(root, value);
    }

    Node* find(int value) {
        return _find(root, value);
    }

    void remove(int value) {
        root = _remove(root, value);
    }

    int max() {
        return _max(root);
    }

    int min() {
        return _min(root);
    }

    void inorder(vector<int>& result) {
        _inorder(root, result);
    }

    void clear() {
        _delete(root);
        root = nullptr;
    }
};

int main() {
    AVLTree avl1{};
    avl1.insert(10); avl1.insert(5); avl1.insert(15); avl1.insert(3);
    vector<int> expected1;
    avl1.inorder(expected1);

    // Copy constructor
    AVLTree avl2(avl1);
    vector<int> expected2;
    avl2.inorder(expected2);
    assert(expected1 == expected2);
    avl1.insert(99);
    assert(avl1.find(99) != nullptr);
    assert(avl2.find(99) == nullptr);

    // Copy assignment operator
    AVLTree avl3;
    avl3.insert(100); avl3.insert(200);
    avl3 = avl2;
    vector<int> expected3;
    avl3.inorder(expected3);
    assert(expected2 == expected3);
    assert(avl3.find(100) == nullptr);

    avl2.remove(5);
    assert(avl2.find(5) == nullptr);
    assert(avl3.find(5) != nullptr);

    avl3 = avl3;
    assert(avl3.find(15) != nullptr);
}
