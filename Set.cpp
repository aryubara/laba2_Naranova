#include "Set.h"
#include <iostream>

using namespace std;

// Хеш-функция DJB2. Превращает строку в индекс массива за O(1)
static unsigned long hash_func(const string& str, int capacity) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacity;
}

// Создание множества (код хэш табл 1)
Set SETCREATE(int initCapacity) {
    Set set;
    set.capacity = initCapacity;
    set.size = 0;
    set.buckets = new FList[initCapacity];
    for (int i = 0; i < initCapacity; ++i) {
        set.buckets[i] = FCREATE(); // Выделяем память под каждый FList
    }
    return set;
}

// Очистка памяти множества
void SETDESTROY(Set& set) {
    for (int i = 0; i < set.capacity; ++i) {
        FDESTROY(set.buckets[i]);
    }
    delete[] set.buckets;
    set.buckets = nullptr;
    set.size = 0;
    set.capacity = 0;
}

// Проверка наличия элемента (SET_AT) за O(1)
bool SET_AT(const Set& set, const string& value) {
    int index = hash_func(value, set.capacity);
    // Ищем строку в бакете с помощь FFIND из 1 лабы
    return FFIND(set.buckets[index], value) != -1;
}

// Добавление элемента (SETADD) за O(1)
bool SETADD(Set& set, string& value) {
    if (SET_AT(set, value)) {
        return false; // Дубликаты в множестве запрещены
    }
    int index = hash_func(value, set.capacity);
    FPUSH_HEAD(set.buckets[index], value); // Кладем в FList
    ++set.size;
    return true;
}

// Удаление элемента (SETDEL) за O(1)
bool SETDEL(Set& set, string& value) {
    if (!SET_AT(set, value)) {
        return false; // Элемента нет в множестве
    }
    int index = hash_func(value, set.capacity);
    FDEL_VALUE(set.buckets[index], value); // функция удаления по значению
    --set.size;
    return true;
}
