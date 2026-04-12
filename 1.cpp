#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

class AVLTree {
    class Node {
        Node* left = nullptr;
        Node* right = nullptr;
        int _value;
        int _height = 1;

        Node(int value): _value(value) {}

        Node(const Node& other): _value(other._value), _height(other._height) {
            if (other.left) left = new Node{*other.left};
            if (other.right) right = new Node{*other.right};
        }
        friend AVLTree;

    public:
        int height() const { return _height; }
        int value() const { return _value; }
    };

    Node* root = nullptr;

    Node* _insert(Node* node, int value) {
        if (!node) return new Node{value};
        if (value > node->_value) node->right = _insert(node->right, value);
        else node->left = _insert(node->left, value);
        return balance(node);
    }

    Node* _find(Node* node, int value) {
        if (!node) return nullptr;
        if (node->_value == value) return node;
        if (value > node->_value) return _find(node->right, value);
        return _find(node->left, value);
    }

    Node* _remove(Node* node, int value) {
        if (!node) return nullptr;
        if (value < node->_value) node->left = _remove(node->left, value);
        else if (value > node->_value) node->right = _remove(node->right, value);
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
                node->_value = min;
                node->right = _remove(node->right, min);
            }
        }
        return balance(node);
    }

    int _min(Node* node) {
        if (!node->left) return node->_value;
        return _min(node->left);
    }

    int _max(Node* node) {
        if (!node->right) return node->_value;
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
        node->_height = std::max(left_height, right_height) + 1;
    }

    int height(Node* node) {
        if (!node) return 0;
        return node->_height;
    }

    void _inorder(Node* node, vector<int>& result) {
        if (!node) return;
        _inorder(node->left, result);
        result.push_back(node->_value);
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

    AVLTree(AVLTree&& other) {
        swap(root, other.root);
    }

    AVLTree& operator=(AVLTree other) {
        swap(root, other.root);
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
    // Move constructor
    AVLTree from1{};
    from1.insert(10); from1.insert(20); from1.insert(30);
    vector<int> expected;
    from1.inorder(expected);
    AVLTree to1{std::move(from1)};
    vector<int> actual;
    to1.inorder(actual);
    assert(actual == expected);

    vector<int> after_move;
    from1.inorder(after_move);
    assert(after_move.empty()); assert(from1.find(10) == nullptr);

    // Move assignment operator
    AVLTree from2{};
    from2.insert(100); from2.insert(200);
    AVLTree to2{};
    to2.insert(142);
    expected.clear();
    from2.inorder(expected);
    to2 = std::move(from2);
    actual.clear();
    to2.inorder(actual);
    assert(actual == expected); assert(to2.find(142) == nullptr);

    after_move.clear();
    from2.inorder(after_move);
    assert(after_move.empty()); assert(from2.find(100) == nullptr);

    // Move self
    to2 = std::move(to2);
    assert(to2.find(100) != nullptr);
}
