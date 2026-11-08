#ifndef SET_H
#define SET_H

#include "List.h" // Используем FList для цепочек коллизий
#include <string>

using namespace std;

// Структура множества на базе Хеш-таблицы
struct Set {
    FList* buckets;   // Массив списков (бакетов)
    int capacity;     // Общий размер таблицы
    int size;         // Количество элементов в множестве
};

Set SETCREATE(int initCapacity = 100);
void SETDESTROY(Set& set);

bool SETADD(Set& set, string& value);
bool SETDEL(Set& set, string& value);
bool SET_AT(const Set& set, const string& value);

#endif
