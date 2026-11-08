#ifndef TASK6_H
#define TASK6_H

#include <string>
#include <vector>
#include <map>

// Ячейка хеш-таблицы (хранит пару ключ-значение)
struct RouteEdge {
    std::string key;          // Ключ
    int value = 0;            // Значение (например, задержка маршрута)
    bool isOccupied = false;  // Занята ли ячейка активным элементом
    bool isDeleted = false;   // Была ли ячейка удалена (для открытой адресации)
};

// Хеш-таблица на базе открытой адресации(обьединяет массив этих ячеек)
struct RouteHashTable {
    RouteEdge* table;
    int capacity;
    int size;
};

// Создание и очистка памяти таблицы
RouteHashTable createRouteTable(int cap = 199);
void destroyRouteTable(RouteHashTable& ht);

// Хеш-функции
unsigned long hash_lp(const std::string& str, int capacity);
unsigned long hash_dh2(const std::string& str, int capacity);

// Операции для Линейного апробирования (принимают пару ключ и значение)
void insertLinear(RouteHashTable& ht, const std::string& key, int value);
bool searchLinear(const RouteHashTable& ht, const std::string& key, int& outValue);
bool removeLinear(RouteHashTable& ht, const std::string& key);

// Операции для Двойного хеширования (принимают пару ключ и значение)
void insertDoubleHash(RouteHashTable& ht, const std::string& key, int value);
bool searchDoubleHash(const RouteHashTable& ht, const std::string& key, int& outValue);
bool removeDoubleHash(RouteHashTable& ht, const std::string& key);

// Подзадание 6.1: Эмпирический анализ для заданного с консоли числа элементов N (вывод только времени)
void runEmpiricalAnalysis(int n);

// Подзадание 6.2: Рекурсивный поиск оптимального маршрута с минимальной задержкой
void findMinDelay(const RouteHashTable& ht, const std::string& current, const std::string& dest, 
                  int currentDelay, std::string currentPath, int& minDelay, std::string& bestPath, 
                  std::map<std::string, bool>& visited);

#endif
