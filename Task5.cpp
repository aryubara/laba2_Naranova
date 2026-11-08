#include "Task5.h"
#include <iostream>

using namespace std;

// Обработчик команды СУБД для вывода развилок дерева
void runBstForks(const vector<string>& args) {
    TNode* bstRoot = nullptr;
    
    // Читаем числа до первого нуля или конца аргументов
    for (const string& s : args) {
        int val = stoi(s);
        if (val == 0) break; // Ноль завершает ввод по условию
        TINSERT_BST(bstRoot, val); // Вызывается из Tree.cpp
    }
    
    cout << "-> ";
    TPRINT_TWO_CHILDREN(bstRoot); // Вызывается из Tree.cpp
    cout << endl;
    
    TDESTROY(bstRoot); // Очищаем временное дерево поиска
}
