#pragma once

#include <cstddef>
#include <vector>

// класс реализует простое сплей дерево для множества целых чисел
// дерево автоматически поднимает последний использованный узел в корень
// что обеспечивает амортизированное логарифмическое время операций
class SplayTree {
public:
    // создаёт пустое дерево
    SplayTree();

    // запрещаем копирование чтобы не дублировать владение узлами
    SplayTree(const SplayTree&) = delete;
    SplayTree& operator=(const SplayTree&) = delete;

    // освобождает всю память
    ~SplayTree();

    // проверяет есть ли значение в дереве и выполняет splay
    bool contains(int value) const;

    // вставляет значение и возвращает true если оно было добавлено
    bool insert(int value);

    // удаляет значение и возвращает true если оно существовало
    bool remove(int value);

    // количество элементов в дереве
    std::size_t size() const;

    // true если дерево пустое
    bool empty() const;

    // возвращает все значения в отсортированном порядке
    std::vector<int> values() const;

private:
    struct Node {
        int value;
        Node* left;
        Node* right;
        Node* parent;
        Node(int v, Node* p = nullptr) : value(v), left(nullptr), right(nullptr), parent(p) {}
    };

    // корень дерева
    mutable Node* root;
    // количество элементов
    std::size_t tree_size;

    // левый поворот вокруг x
    void rotateLeft(Node* x) const;
    // правый поворот вокруг x
    void rotateRight(Node* x) const;
    // поднимает узел к корню
    void splay(Node* x) const;
    // ищет узел со значением или возвращает последний посещённый узел
    Node* findNode(int value) const;
    // рекурсивно удаляет все узлы
    void destroy(Node* node);
    // обход в глубину для получения значений
    void inorder(Node* node, std::vector<int>& out) const;
};
