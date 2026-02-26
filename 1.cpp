#include <iostream>
using namespace std;

class AVLTree {
    struct Node {
        Node* left = nullptr;
        Node* right = nullptr;
        int value;
        int height = 1;

        Node(int value) {
            this->value = value;
        }

        Node(const Node& other): left(other.left), right(other.right), value(other.value), height(other.height) {}

        Node& operator=(const Node& other) {
            if (this != &other) {
                left = other.left;
                right = other.right;
                value = other.value;
                height = other.height;
            }
            return *this;
        }
    };

    Node* root = nullptr;

    AVLTree(const AVLTree &other): root(other.root) {}

    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            root = other.root;
        }
        return *this;
    }

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
                return node;
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

    void _inorder(Node* node) {
        if (!node) return;
        _inorder(node->left);
        cout << node->value << " ";
        _inorder(node->right);
    }

public:
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

    void inorder() {
        _inorder(root);
        cout << endl;
    }
};

int main() {
    AVLTree avl{};
    avl.insert(9);
    avl.insert(4);
    avl.insert(10);
    avl.insert(2);
    avl.insert(6);
    avl.insert(17);
    avl.insert(5);
    avl.insert(7);
    avl.inorder();
    cout << avl.find(5)->height << endl;
    avl.remove(4);
    avl.inorder();
    cout << avl.find(5)->height << endl;
    cout << avl.find(6)->height << endl;
    avl.insert(8);
    cout << avl.find(6)->height << endl;
    avl.inorder();
    cout << avl.max() << endl;
    avl.remove(17);
    cout << avl.max() << endl;
    cout << avl.min() << endl;
}
