#include "Tree.h"
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

// Создание пустого дерева
Tree TCREATE() {
    Tree tree;
    tree.root = nullptr;
    tree.size = 0;
    return tree;
}

// Рекурсивное уничтожение памяти всего дерева
void TDESTROY(TNode*& node) {
    if (node == nullptr) {
        return;
    }
    TDESTROY(node->left);
    TDESTROY(node->right);
    delete node;
    node = nullptr;
}

// Добавление элемента на самый первый свободный уровень (сверху-вниз, слева-направо)
void TINSERT(Tree& tree, int value) {
    TNode* newNode = new TNode{value, nullptr, nullptr};
    
    if (tree.root == nullptr) {
        tree.root = newNode;
        tree.size = 1;
        return;
    }
    
    // Используем стандартную очередь для поиска места вставки по уровням 
    queue<TNode*> q;
    q.push(tree.root);
    
    while (!q.empty()) {
        TNode* curr = q.front();
        q.pop();

        if (curr->left == nullptr) {
            curr->left = newNode;
            ++tree.size;
            return;
        } else {
            q.push(curr->left);
        }

        if (curr->right == nullptr) {
            curr->right = newNode;
            ++tree.size;
            return;
        } else {
            q.push(curr->right);
        }
    }
}

// Проверка наличия числа в дереве (обычный поиск обходом)
bool TFIND(const Tree& tree, int value) {
    if (tree.root == nullptr) {
        return false;
    }
    
    queue<TNode*> q;
    q.push(tree.root);
    
    while (!q.empty()) {
        TNode* curr = q.front();
        q.pop();
        
        if (curr->key == value) {
            return true; 
        }
        if (curr->left != nullptr) {
            q.push(curr->left);
        }
        if (curr->right != nullptr) {
            q.push(curr->right);
        }
    }
    return false;
}

// Удаление самого последнего добавленного элемента дерева
bool TDEL(Tree& tree, int value) {
    if (tree.root == nullptr) {
        cout << "Нет значений" << endl;
        return false;
    }
    
    // Если корень один в дереве
    if (tree.root->left == nullptr && tree.root->right == nullptr) {
        delete tree.root;
        tree.root = nullptr;
        tree.size = 0;
        cout << "Значение удалено" << endl;
        return true;
    }
    
    queue<TNode*> q;
    q.push(tree.root);
    
    TNode* curr = nullptr;
    TNode* lastNodeParent = nullptr;
    TNode* lastNode = nullptr;

    // 1 Находим самый последний узел в дереве на нижнем уровне
    while (!q.empty()) {
        curr = q.front();
        q.pop();
        
        if (curr->left != nullptr) {
            q.push(curr->left);
            lastNodeParent = curr;
            lastNode = curr->left;
        }
        if (curr->right != nullptr) {
            q.push(curr->right);
            lastNodeParent = curr;
            lastNode = curr->right;
        }
    }

    // 2 Удаляем связь родителя с этим последним узлом
    if (lastNodeParent != nullptr) {
        if (lastNodeParent->right == lastNode) {
            lastNodeParent->right = nullptr;
        } else {
            lastNodeParent->left = nullptr;
        }
        delete lastNode;
        --tree.size;
        cout << "Значение удалено" << endl;
        return true;
    }
    
    return false;
}

// Прямой обход (Pre-order): Корень -> Левый -> Правый
void TGET_PREORDER(const TNode* node) {
    if (!node) return;
    cout << node->key << " ";
    TGET_PREORDER(node->left);
    TGET_PREORDER(node->right);
}

// Симметричный обход (In-order): Левый -> Корень -> Правый
void TGET_INORDER(const TNode* node) {
    if (!node) return;
    TGET_INORDER(node->left);
    cout << node->key << " ";
    TGET_INORDER(node->right);
}

// Обратный обход (Post-order): Левый -> Правый -> Корень
void TGET_POSTORDER(const TNode* node) {
    if (!node) return;
    TGET_POSTORDER(node->left);
    TGET_POSTORDER(node->right);
    cout << node->key << " ";
}

// Проверка: является ли дерево почти полным (Complete Binary Tree)
bool TIS_COMPLETE(const Tree& tree) {
    if (tree.root == nullptr || tree.root->key == -999) {
        return true; 
    }

    queue<TNode*> q;
    q.push(tree.root);
    
    bool reachedEnd = false; // Флаг первой встреченной пустоты

    while (!q.empty()) {
        TNode* curr = q.front();
        q.pop();

        // Проверяем левую ветку (узел считается пустым, если он nullptr или равен -999)
        if (curr->left != nullptr && curr->left->key != -999) {
            if (reachedEnd) return false; // Нашли реальный узел после пустоты - дерево НЕ полное
            q.push(curr->left);
        } else {
            reachedEnd = true; // Зафиксировали первую пустоту / заглушку
        }

        // Проверяем правую ветку
        if (curr->right != nullptr && curr->right->key != -999) {
            if (reachedEnd) return false; // Нашли реальный узел после пустоты — дерево НЕ полное!
            q.push(curr->right);
        } else {
            reachedEnd = true; // Зафиксировали первую пустоту / заглушку
        }
    }
    return true; 
}

// Проверка: является ли дерево строго полным (Full Binary Tree)
bool TIS_FULL(const Tree& tree) {
    if (tree.root == nullptr || tree.root->key == -999) {
        return true; 
    }

    queue<TNode*> q;
    q.push(tree.root);

    while (!q.empty()) {
        TNode* curr = q.front();
        q.pop();

        // Извлекаем реальное состояние детей с учетом заглушек -999
        bool hasLeft = (curr->left != nullptr && curr->left->key != -999);
        bool hasRight = (curr->right != nullptr && curr->right->key != -999);

        // Если у узла один ребенок есть, а второго нет — дерево НЕ строго полное
        if (hasLeft != hasRight) {
            return false;
        }

        if (hasLeft) q.push(curr->left);
        if (hasRight) q.push(curr->right);
    }
    return true;
}
void TGET_LEVELORDER(const Tree& tree) { // вывод по уровням
    if (tree.root == nullptr) return;
    queue<TNode*> q;
    q.push(tree.root);
    while (!q.empty()) {
        TNode* curr = q.front();
        q.pop();
        if (curr->key != -999) { // пропускаем технические заглушки, если они есть
            cout << curr->key << " ";
        }
        if (curr->left != nullptr) q.push(curr->left);
        if (curr->right != nullptr) q.push(curr->right);
    }
}
// задание 5 (2 лабы) (Вариант 4: Вывод развилок)

// Рекурсивная вставка в бинарное дерево поиска (BST)
void TINSERT_BST(TNode*& root, int value) {
    if (root == nullptr) {
        root = new TNode{value, nullptr, nullptr};
        return;
    }
    if (value < root->key) {
        TINSERT_BST(root->left, value);
    } else if (value > root->key) {
        TINSERT_BST(root->right, value);
    }
    // Если значение уже есть, дубликаты в BST обычно пропускаем
}

// Симметричный обход (для развилок) с выводом вершин, имеющих ровно 2 детей
void TPRINT_TWO_CHILDREN(const TNode* node) {
    if (node == nullptr) return;
    
    TPRINT_TWO_CHILDREN(node->left);  // Сначала обходим левое поддерево (возрастание)
    
    // Проверяем условие: у вершины есть и левый, и правый ребенок
    if (node->left != nullptr && node->right != nullptr) {
        cout << node->key << " ";
    }
    
    TPRINT_TWO_CHILDREN(node->right); // Затем обходим правое поддерево
}
