#include "Task2.h"
#include <iostream>

using namespace std;

// Обработка обавления элемента в хеш-таблицу множества
bool handleSetAdd(Set& set, const string& val) {
    string copy = val;
    if (SETADD(set, copy)) {
        cout << "-> OK" << endl;
    } else {
        cout << "-> Element already exists" << endl;
    }
    return true;
}

// Обработка удаления элемента из хеш-таблицу множества
bool handleSetDel(Set& set, const string& val) {
    string copy = val;
    if (SETDEL(set, copy)) {
        cout << "-> OK" << endl;
    } else {
        cout << "-> Element not found" << endl;
    }
    return true;
}

// Проверка существования элемента в множестве
bool handleSetAt(const Set& set, const string& val) {
    cout << "-> " << (SET_AT(set, val) ? "TRUE" : "FALSE") << endl;
    return true;
}
