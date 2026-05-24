#include "SplayTree.hpp"

// конструктор инициализирует пустой корень и нулевой размер
SplayTree::SplayTree() : root(nullptr), tree_size(0) {}

// деструктор освобождает память
SplayTree::~SplayTree() {
    destroy(root);
    root = nullptr;
    tree_size = 0;
}

// рекурсивно освобождает узлы
void SplayTree::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// возвращает количество элементов
std::size_t SplayTree::size() const {
    return tree_size;
}

// true если дерево пустое
bool SplayTree::empty() const {
    return tree_size == 0;
}

// левый поворот вокруг узла x
void SplayTree::rotateLeft(Node* x) const {
    Node* y = x->right;
    if (!y) return;
    // переназначаем правое поддерево x
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }
    // переносим y наверх
    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    // делаем x левым потомком y
    y->left = x;
    x->parent = y;
}

// правый поворот вокруг узла x
void SplayTree::rotateRight(Node* x) const {
    Node* y = x->left;
    if (!y) return;
    x->left = y->right;
    if (y->right) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

// поднимает узел к корню
void SplayTree::splay(Node* x) const {
    if (!x) return;
    while (x->parent) {
        Node* p = x->parent;
        Node* g = p->parent;
        if (!g) {
            // родитель корень
            if (x == p->left) {
                rotateRight(p);
            } else {
                rotateLeft(p);
            }
        } else if (x == p->left && p == g->left) {
            // zig-zig слева
            rotateRight(g);
            rotateRight(p);
        } else if (x == p->right && p == g->right) {
            // zig-zig справа
            rotateLeft(g);
            rotateLeft(p);
        } else if (x == p->left && p == g->right) {
            // zig-zag
            rotateRight(p);
            rotateLeft(g);
        } else {
            rotateLeft(p);
            rotateRight(g);
        }
    }
}

// ищет узел с данным значением или последний посещённый узел
SplayTree::Node* SplayTree::findNode(int value) const {
    Node* current = root;
    Node* parent = nullptr;
    while (current) {
        parent = current;
        if (value == current->value) {
            return current;
        } else if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return parent;
}

// проверяет наличие значения в дереве и поднимает соответствующий узел
bool SplayTree::contains(int value) const {
    Node* x = findNode(value);
    bool found = x && x->value == value;
    if (x) {
        splay(x);
    }
    return found;
}

// вставляет значение если его нет
bool SplayTree::insert(int value) {
    if (!root) {
        root = new Node(value);
        tree_size = 1;
        return true;
    }
    Node* x = findNode(value);
    if (x && x->value == value) {
        splay(x);
        return false;
    }
    // создаём новый узел с указателем на найденного предка
    Node* newNode = new Node(value, x);
    if (value < x->value) {
        x->left = newNode;
    } else {
        x->right = newNode;
    }
    splay(newNode);
    tree_size++;
    return true;
}

// удаляет значение если оно есть
bool SplayTree::remove(int value) {
    Node* x = findNode(value);
    // если нет такого узла, поднимаем последний найденный
    if (!x || x->value != value) {
        if (x) {
            splay(x);
        }
        return false;
    }
    // splay удаляемый узел в корень
    splay(x);
    Node* left = root->left;
    Node* right = root->right;
    root->left = nullptr;
    root->right = nullptr;
    delete root;
    tree_size--;
    if (!left) {
        root = right;
        if (root) root->parent = nullptr;
    } else {
        root = left;
        root->parent = nullptr;
        // находим максимальный узел в левом поддереве
        Node* m = left;
        while (m->right) {
            m = m->right;
        }
        // поднимаем его чтобы соединить с правым поддеревом
        splay(m);
        // присоединяем правое поддерево
        root->right = right;
        if (right) right->parent = root;
    }
    return true;
}

// возвращает элементы в отсортированном порядке
std::vector<int> SplayTree::values() const {
    std::vector<int> out;
    out.reserve(tree_size);
    inorder(root, out);
    return out;
}

// рекурсивный обход
void SplayTree::inorder(Node* node, std::vector<int>& out) const {
    if (!node) return;
    inorder(node->left, out);
    out.push_back(node->value);
    inorder(node->right, out);
}
