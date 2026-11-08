#ifndef TREE_H
#define TREE_H

#include <string>
#include <fstream>
using namespace std;

// Структура одного узла дерева
struct TNode {
    int key;          // Само число
    TNode* left;      // Ссылка на левого потомка
    TNode* right;     // Ссылка на правого потомка
};

// Структура-обёртка для дерева
struct Tree {
    TNode* root;      // Самый верхний (главный) корень
    int size;         // Количество элементов в дереве
};

// Основные функции управления деревом 
Tree TCREATE();
void TDESTROY(TNode*& node);

void TINSERT(Tree& tree, int value);
bool TFIND(const Tree& tree, int value);
bool TDEL(Tree& tree, int value);

// Функции обхода 
void TGET_PREORDER(const TNode* node);
void TGET_INORDER(const TNode* node);
void TGET_POSTORDER(const TNode* node);

bool TIS_COMPLETE(const Tree& tree); // проверка
bool TIS_FULL(const Tree& tree); 
void TGET_LEVELORDER(const Tree& tree);
// Новые функции для Варианта 4(2 лабы) (Дерево поиска и вывод развилок)
void TINSERT_BST(TNode*& root, int value);
void TPRINT_TWO_CHILDREN(const TNode* node);




#endif
