#include <iostream>
#include <cassert>
#include <vector>
#include<memory>
using namespace std;

template <typename T>
class AVLTree {
    class Node {
        std::shared_ptr<Node> left = nullptr;
        std::shared_ptr<Node> right = nullptr;
        T _value;
        int _height = 1;

    public:
        Node(T value): _value(value) {}

        Node(const Node& other): _value(other._value), _height(other._height) {
            if (other.left) left = make_shared<Node>(*other.left);
            if (other.right) right = make_shared<Node>(*other.right);
        }
        friend AVLTree;

        int height() const { return _height; }
        T value() const { return _value; }
    };

    std::shared_ptr<Node> root = nullptr;

    std::shared_ptr<Node> _insert(std::shared_ptr<Node> node, T value) {
        if (!node) return make_shared<Node>(value);
        if (value > node->_value) node->right = _insert(node->right, value);
        else node->left = _insert(node->left, value);
        return balance(node);
    }

    std::shared_ptr<Node> _find(std::shared_ptr<Node> node, T value) {
        if (!node) return nullptr;
        if (node->_value == value) return node;
        if (value > node->_value) return _find(node->right, value);
        return _find(node->left, value);
    }

    std::shared_ptr<Node> _remove(std::shared_ptr<Node> node, T value) {
        if (!node) return nullptr;
        if (value < node->_value) node->left = _remove(node->left, value);
        else if (value > node->_value) node->right = _remove(node->right, value);
        else {
            if (!node->left and !node->right) {
                node = nullptr;
                return node;
            }
            else if (!node->left or !node->right) {
                std::shared_ptr<Node> res;
                if (node->left) res = node->left;
                else res = node->right;
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

    T _min(std::shared_ptr<Node> node) {
        if (!node->left) return node->_value;
        return _min(node->left);
    }

    T _max(std::shared_ptr<Node> node) {
        if (!node->right) return node->_value;
        return _max(node->right);
    }

    std::shared_ptr<Node> balance(std::shared_ptr<Node> node) {
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

    std::shared_ptr<Node> right_rotate(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> left = node->left;
        node->left = left->right;
        left->right = node;
        calc_height(node);
        calc_height(left);
        return left;
    }

    std::shared_ptr<Node> left_rotate(std::shared_ptr<Node> node) {
        std::shared_ptr<Node> right = node->right;
        node->right = right->left;
        right->left = node;
        calc_height(node);
        calc_height(right);
        return right;
    }

    int get_balance(std::shared_ptr<Node> node) {
        return height(node->right) - height(node->left);
    }

    void calc_height(std::shared_ptr<Node> node) {
        int left_height = height(node->left);
        int right_height = height(node->right);
        node->_height = std::max(left_height, right_height) + 1;
    }

    int height(std::shared_ptr<Node> node) {
        if (!node) return 0;
        return node->_height;
    }

    void _inorder(std::shared_ptr<Node> node, vector<T>& result) {
        if (!node) return;
        _inorder(node->left, result);
        result.push_back(node->_value);
        _inorder(node->right, result);
    }

public:
    AVLTree() = default;

    AVLTree(std::initializer_list<T> il) {
        for(auto&& x: il) {
            insert(x);
        }
    }

    AVLTree(const AVLTree &other) {
        if (other.root) root = make_shared<Node>(*other.root);
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

    std::shared_ptr<Node> find(T value) {
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
        root = nullptr;
    }

    class iterator {
        vector<std::shared_ptr<Node>> path;
    public:
        iterator(std::shared_ptr<Node> root) {
            go_left(root);
        }

        void go_left(std::shared_ptr<Node> node) {
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
            std::shared_ptr<Node> leaf = path.back();
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
    AVLTree<int> int_tree{100, 50, 25, 75, 65, 85, 150, 125, 175};
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
