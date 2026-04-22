#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

template <typename T>
class AVLTree {
    class Node {
        Node* left = nullptr;
        Node* right = nullptr;
        T _value;
        int _height = 1;

        Node(T value): _value(value) {}

        Node(const Node& other): _value(other._value), _height(other._height) {
            if (other.left) left = new Node{*other.left};
            if (other.right) right = new Node{*other.right};
        }
        friend AVLTree;

    public:
        int height() const { return _height; }
        T value() const { return _value; }
    };

    Node* root = nullptr;

    Node* _insert(Node* node, T value) {
        if (!node) return new Node{value};
        if (value > node->_value) node->right = _insert(node->right, value);
        else node->left = _insert(node->left, value);
        return balance(node);
    }

    Node* _find(Node* node, T value) {
        if (!node) return nullptr;
        if (node->_value == value) return node;
        if (value > node->_value) return _find(node->right, value);
        return _find(node->left, value);
    }

    Node* _remove(Node* node, T value) {
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
                T min = _min(node->right);
                node->_value = min;
                node->right = _remove(node->right, min);
            }
        }
        return balance(node);
    }

    T _min(Node* node) {
        if (!node->left) return node->_value;
        return _min(node->left);
    }

    T _max(Node* node) {
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

    void _inorder(Node* node, vector<T>& result) {
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

    void insert(T value) {
        root = _insert(root, value);
    }

    Node* find(T value) {
        return _find(root, value);
    }

    void remove(T value) {
        root = _remove(root, value);
    }

    T max() {
        return _max(root);
    }

    T min() {
        return _min(root);
    }

    void inorder(vector<T>& result) {
        _inorder(root, result);
    }

    void clear() {
        _delete(root);
        root = nullptr;
    }

    class iterator {
        vector<Node*> path;
    public:
        iterator(Node* root) {
            go_left(root);
        }

        void go_left(Node* node) {
            while (node) {
                path.push_back(node);
                node = node->left;
            }
        }

        bool operator!=(const iterator& other) const {
            if (path.empty() && other.path.empty()) return false;
            if (path.empty() != other.path.empty()) return true;
            return path.back() != other.path.back();
        }

        iterator& operator++() {
            Node* leaf = path.back();
            path.pop_back();
            go_left(leaf->right);
            return *this;
        }

        const T* operator->() {
            return &path.back()->_value;
        }

        const T& operator*() {
            return path.back()->_value;
        }
    };

    iterator begin() {
        return iterator(root);
    }

    iterator end() {
        return iterator(nullptr);
    }
};

int main() {
    AVLTree<int> int_tree{};
    int_tree.insert(100); int_tree.insert(50); int_tree.insert(25); int_tree.insert(75); int_tree.insert(65);
    int_tree.insert(85); int_tree.insert(150); int_tree.insert(125); int_tree.insert(175);
    auto it = int_tree.begin();
    auto end = int_tree.end();
    for (; it != end; ++it) {
        cout << *it << " ";
    }
    cout << endl;
    for (auto&& i : int_tree) {
        cout << i << " ";
    }
}
